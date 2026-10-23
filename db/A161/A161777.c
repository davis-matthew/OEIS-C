/*
 * A161777  n-th nonprime*(n-th nonprime-1)/2.
 * https://oeis.org/A161777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n‑th_nonprime * (n‑th_nonprime - 1) / 2
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the same as A161777: a(n) = triangular(n‑th_nonprime). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* ---------- static cache of n‑th non‑primes ---------- */
static seq_t *nonprimes = NULL;       /* array of discovered non‑prime values   */
static idx_t  count_nonprimes = 0;    /* number of entries currently valid      */
static size_t cap_nonprimes = 0;      /* allocated capacity                       */
static idx_t  next_to_test = 1;       /* next integer to test for primality      */

/* Ensure that the first `target` non‑primes are stored in `nonprimes`. */
static void ensure_nonprimes(idx_t target)
{
    if (nonprimes == NULL) {
        cap_nonprimes = 16;          /* initial capacity guess */
        nonprimes = (seq_t *)malloc(cap_nonprimes * sizeof(seq_t));
        if (!nonprimes) abort();
    }

    while (count_nonprimes < target) {
        /* enlarge array if necessary */
        if (count_nonprimes >= cap_nonprimes) {
            cap_nonprimes <<= 1;        /* double the capacity */
            seq_t *tmp = (seq_t *)realloc(nonprimes, cap_nonprimes * sizeof(seq_t));
            if (!tmp) abort();
            nonprimes = tmp;
        }

        /* 1 is treated as a non‑prime */
        if (next_to_test == 1) {
            nonprimes[count_nonprimes++] = (seq_t)1;
            ++next_to_test;
            continue;
        }

        /* test other integers for primality */
        if (!is_prime((idx_t)next_to_test)) {
            nonprimes[count_nonprimes++] = (seq_t)next_to_test;
        }
        ++next_to_test;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n < 1, safe fallback */

    ensure_nonprimes(n);             /* make sure we have at least n entries */

    seq_t np = nonprimes[(size_t)(n - 1)];   /* nth non‑prime (0‑based index) */
    return np * (np - (seq_t)1) / (seq_t)2;   /* triangular number */
}
