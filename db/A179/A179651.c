/*
 * A179651  Difference between consecutive practical numbers.
 * https://oeis.org/A179651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A005153(n+1) - A005153(n)
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 119 terms through n = 8081 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the difference between consecutive practical numbers. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>               /* for sprintf */
#include <string.h>
#include "../utils/mem.h"

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

/* -------------------------------------------------------------------------- */
static bool is_practical_u64(uint64_t n)
{
    if (n == 1)                 /* 1 is practical */
        return true;
    if ((n & 1ULL) != 0)        /* practical numbers >1 are even */
        return false;

    uint64_t tmp = n;
    unsigned exp2 = 0;

    /* separate out the power of two */
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        exp2++;
    }

    /* sigma(2^exp2) = 2^(exp2+1) - 1 */
    uint64_t sum = 1ULL;
    uint64_t power = 1ULL;
    for (unsigned i = 0; i < exp2 + 1; ++i)
        power *= 2ULL;
    sum = power - 1ULL;

    /* If only a power of two, it is practical */
    if (tmp == 1ULL)
        return true;

    /* factor remaining odd part using trial division */
    uint64_t p = 3ULL;
    while (p * p <= tmp) {
        if (tmp % p == 0) {
            unsigned exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }

            /* Practicality test: the next prime must not exceed sum+1 */
            if (p > sum + 1ULL)
                return false;

            /* sigma(p^exp) = (p^(exp+1)-1)/(p-1) */
            uint64_t p_pow = 1ULL;
            for (unsigned i = 0; i < exp + 1; ++i)
                p_pow *= p;
            uint64_t term = (p_pow - 1ULL) / (p - 1ULL);
            sum *= term;
        }
        p += 2ULL;
    }

    /* any remaining prime factor >1 */
    if (tmp > 1ULL) {
        uint64_t r = tmp;
        if (r > sum + 1ULL)
            return false;
        uint64_t r_pow = r * r;
        uint64_t term   = (r_pow - 1ULL) / (r - 1ULL);
        sum *= term;
    }

    return true;
}

/* -------------------------------------------------------------------------- */
/*  Utility: test whether a 64‑bit integer is practical.                       */

/* -------------------------------------------------------------------------- */
/*  Storage of discovered practical numbers                                 */
/* -------------------------------------------------------------------------- */
static seq_t *practical_numbers = NULL; /* array of seq_t values */
static size_t practical_len = 0;        /* number of stored values */
static size_t practical_cap = 0;        /* current allocated capacity */
static uint64_t next_candidate = 2;     /* next integer to test for practicality */

/* -------------------------------------------------------------------------- */
/*  Ensure the array has room for at least `needed` more elements             */
/* -------------------------------------------------------------------------- */
static void ensure_capacity(size_t needed)
{
    if (needed <= practical_cap)
        return;
    size_t new_cap = (practical_cap == 0) ? 16 : practical_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    practical_numbers = mem_resize(seq_t, practical_numbers, new_cap);
    practical_cap = new_cap;
}

/* -------------------------------------------------------------------------- */
/*  Generate and store the next practical number                               */
/* -------------------------------------------------------------------------- */
static void generate_next_practical(void)
{
    while (true) {
        if (is_practical_u64(next_candidate)) {
            /* convert the 64‑bit candidate to seq_t for storage */
            char buf[32];
            sprintf(buf, "%llu", (unsigned long long)next_candidate);
            seq_t val = seq_from_str(buf);

            ensure_capacity(practical_len + 1);
            practical_numbers[practical_len++] = val;

            next_candidate += 2ULL;   /* skip to next even candidate */
            break;
        }
        next_candidate += 2ULL;
    }
}

/* -------------------------------------------------------------------------- */
/*  Public function: compute a(n) = A005153(n+1) - A005153(n)                 */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise with the first practical number if empty */
    if (practical_len == 0) {
        practical_numbers = mem_new(seq_t, 16);
        practical_cap     = 16;
        practical_len     = 1;
        /* 1 is the first practical number */
        practical_numbers[0] = seq_from_str("1");
        next_candidate = 2ULL;
    }

    /* Ensure we have stored at least n+1 practical numbers (indices 0..n) */
    while (practical_len <= (size_t)n)
        generate_next_practical();

    /* a(n) = practical[n] - practical[n-1] */
    seq_t prev = practical_numbers[(size_t)n - 1];
    seq_t curr = practical_numbers[(size_t)n];
    return curr - prev;
}
