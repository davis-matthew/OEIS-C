/*
 * A179651  Difference between consecutive practical numbers.
 * https://oeis.org/A179651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A005153(n+1) - A005153(n)
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   is_practical_u64, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the difference between consecutive practical numbers. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>               /* for sprintf */
#include "../utils/seq_from_str.h" /* seq_from_str() converts a string to seq_t */
#include "../utils/is_practical_u64.h"

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
    practical_numbers = (seq_t *)realloc(practical_numbers, new_cap * sizeof(seq_t));
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
        practical_numbers = (seq_t *)malloc(16 * sizeof(seq_t));
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
