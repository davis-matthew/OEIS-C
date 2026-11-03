/*
 * A228019  Composite numbers whose sum of digits is a composite number.
 * https://oeis.org/A228019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9983 (2026-11-03)
 * Helpers:   digit_sum, is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A228019: composite numbers whose digit sum is composite */
/* The implementation iteratively searches for the required numbers
   and memoises the results so that calls may arrive in any order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* cache for memoisation */
static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* current allocated capacity */
static seq_t last_term = 0;      /* last term stored in the cache */

/* ensure the cache can store at least `min_cap` elements */
static void ensure_capacity(size_t min_cap)
{
    if (cache_cap < min_cap)
        grow_seq(&cache, &cache_cap, min_cap);
}

seq_t a(idx_t n)
{
    /* any n < 1 is outside the defined offset; return 0 as a safe default */
    if (n < 1)
        return (seq_t)0;

    /* grow the cache until we have at least n terms */
    while (cache_len < (size_t)n)
    {
        seq_t cand = last_term;
        while (true)
        {
            cand = cand + (seq_t)1;                 /* next integer */

            /* skip 0 and 1 – they are not composite */
            if (cand < (seq_t)2)
                continue;

            /* skip primes */
            if (is_prime_big(cand))
                continue;

            /* evaluate the digit sum */
            seq_t sum = digit_sum(cand);

            /* a composite sum means >1 and not prime (sum>1 && !is_prime_small) */
            if (sum > (seq_t)1 && !is_prime_seq(sum))
            {
                /* found the next required number */
                ensure_capacity(cache_len + 1);
                cache[cache_len++] = cand;
                last_term = cand;
                break; /* outer cand loop */
            }
        }
    }

    return cache[(size_t)n - 1];
}
