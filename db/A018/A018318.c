/*
 * A018318  Divisors of 174.
 * https://oeis.org/A018318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 174 in ascending order */
#include <stddef.h>    /* size_t */

/* The seq_cmp utility compares two seq_t values in ascending order.
   It is defined in ../utils/seq_cmp.h. */
#include "../utils/seq_cmp.h"

seq_t a(idx_t n)
{
    /* Sequence only contains 8 terms.  Return 0 for out‑of‑range indices. */
    if (n == 0) {
        return (seq_t)0;
    }

    const idx_t N = 174;          /* constant whose divisors are listed */
    seq_t  small[8];              /* divisors ≤ sqrt(N), ascending */
    seq_t  large[8];              /* divisors > sqrt(N), descending */
    size_t small_cnt = 0;
    size_t large_cnt = 0;

    /* Enumerate divisors in a single pass. */
    for (idx_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            small[small_cnt++] = (seq_t)i;

            idx_t j = N / i;
            if (j != i) {
                large[large_cnt++] = (seq_t)j;   /* j decreases as i increases */
            }
        }
    }

    /* Total number of divisors. */
    size_t total = small_cnt + large_cnt;
    if ((size_t)(n - 1) >= total) {
        return (seq_t)0;                       /* out of range */
    }

    /* Translate the n-th 1‑based index to the correct element: */
    if ((size_t)(n - 1) < small_cnt) {
        /* in the small (ascending) part */
        return small[n - 1];
    } else {
        /* in the large (descending) part – reverse to get ascending order */
        size_t pos = (size_t)(n - 1) - small_cnt;
        return large[large_cnt - pos - 1];
    }
}
