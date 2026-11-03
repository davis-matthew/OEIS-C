/*
 * A106920  Primes of the form 3x^2-xy+5y^2, with x and y nonnegative.
 * https://oeis.org/A106920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1188, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9813 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106920: primes of the form 3x^2 – x*y + 5y^2 (x,y ≥ 0) */
/* The function returns the n‑th prime (1‑based) satisfying the representation. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt.h"
#include "../utils/mem.h"

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

/* Test whether a prime p can be written as 3x^2 – x*y + 5y^2 with x,y ≥ 0.
   For each y with 12p – 59y² ≥ 0 we solve 3x² – yx + (5y² – p) = 0.
   The discriminant is D = 12p – 59y².  A solution exists iff D is a perfect square
   and either (y + √D) or (y – √D) is divisible by 6.  */
static bool form_rep_prime(uint64_t p)
{
    const uint64_t limit = 12 * p;
    for (uint64_t y = 0; ; ++y) {
        uint64_t ysq = y * y;
        uint64_t t   = 59 * ysq;
        if (t > limit) break;
        uint64_t D   = limit - t;
        uint64_t sD  = isqrt(D);
        if (sD * sD == D) {
            if (((y + sD) % 6ULL) == 0ULL || ((y >= sD) && ((y - sD) % 6ULL) == 0ULL))
                return true;
        }
    }
    return false;
}

/* Return the n-th prime of the form 3x^2 – x*y + 5y^2.  */
seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *seq_arr   = NULL;
    static size_t seq_cap   = 0;
    static size_t seq_size  = 0;

    /* Next odd integer to test as a prime candidate. */
    static uint64_t next_candidate = 3U;

    /* Generate terms until we have at least n of them. */
    while (seq_size < (size_t)n) {
        uint64_t cand = next_candidate;
        next_candidate += 2;              /* skip even numbers */

        if (is_prime_u64(cand) && form_rep_prime(cand)) {
            push_seq(&seq_arr, &seq_cap, &seq_size, (seq_t)cand);
        }
    }

    /* n is 1‑based; undefined for n==0 but return 0 for safety. */
    if (n == 0) return 0;
    return seq_arr[n - 1];
}
