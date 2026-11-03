/*
 * A107835  Slowest increasing sequence where a(n)-(first digit of a(n)) is
 *          prime.
 * https://oeis.org/A107835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the smallest integer > a(n-1) such that a(n) -
 *            first_digit(a(n)) is prime. The sequence is defined for n ≥ 1
 *            with offset 1. This implementation memoizes the terms and
 *            expands the table on demand
 * Range:     u8 n <= 49, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

/* Ensure an array of seq_t is large enough to hold indexes [0..min_needed] */
static void ensure_seq_array(seq_t **arr, size_t *cap, size_t min_needed)
{
    if (*cap <= min_needed) {
        size_t newcap = (*cap) ? (*cap) * 2 : 16;
        if (newcap <= min_needed) newcap = min_needed + 1;
        *arr = mem_resize(seq_t, *arr, newcap);
        for (size_t i = *cap; i < newcap; ++i) (*arr)[i] = 0;
        *cap = newcap;
    }
}

/* Return the most significant decimal digit of x.  Works for any
   seq_t type that supports division and comparison with an integer
   constant. */
static seq_t first_digit(seq_t x)
{
    seq_t d = x;
    while (d >= (seq_t)10)
        d /= (seq_t)10;
    return d;
}

seq_t a(idx_t n)
{
    static seq_t *tbl  = NULL;   /* 1‑based array of computed terms   */
    static size_t cap = 0;       /* allocated capacity                */
    static size_t sz  = 0;       /* number of terms stored            */

    /* Expand the table until we have at least n terms. */
    while (sz < n) {
        seq_t val;
        if (sz == 0) {                 /* a(1) = 12 */
            val = (seq_t)12;
        } else {
            seq_t cand = tbl[sz - 1] + (seq_t)1;
            for (;;) {
                seq_t fd   = first_digit(cand);
                seq_t diff = cand - fd;
                if (is_prime_seq(diff)) {
                    val = cand;
                    break;
                }
                cand += (seq_t)1;
            }
        }

        /* Ensure the array is large enough for the new element. */
        ensure_seq_array(&tbl, &cap, sz + 1);
        tbl[sz++] = val;
    }

    return tbl[n - 1];
}
