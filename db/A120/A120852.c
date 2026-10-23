/*
 * A120852  Triangle read by rows, where t(n,1) = 1, t(n,m) = t(n,m-1) +
 *          (largest noncomposite {1 or prime} in row {n-1}).
 * https://oeis.org/A120852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the nth element of the triangle read by rows, where
 *            t(1,1)=1, and for n>1, t(n,1)=1, t(n,m)=t(n,m-1)+L_{n-1}, with
 *            L_{n-1} = the largest prime or 1 in row (n-1)
 * Range:     u8 n <= 27, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------- Standard includes -------------- */
#include <stdbool.h>
#include <stddef.h>

/* -------------- Utility includes -------------- */
#include "../utils/is_prime_big.h"

/* -------------- Sequence function -------------- */
seq_t a(idx_t n) {
    seq_t diff = 1;      /* L_{previous row}  (difference for current row) */
    idx_t  row_len = 1;  /* length of current row */
    idx_t  idx = 1;      /* current index in the flattened triangle */

    while (true) {
        /* Generate the current row */
        seq_t val = 1;
        seq_t largest_prime = 1;   /* 1 is considered the default non‑composite */

        for (idx_t m = 1; m <= row_len; ++m) {
            /* If we have reached the requested index, return the value */
            if (idx == n) {
                return val;
            }

            /* Update largest prime seen in this row */
            if (is_prime_big(val)) {
                largest_prime = val;   /* since values increase, this will be the largest prime */
            }

            /* Prepare value for next element in the row */
            val += diff;
            ++idx;
        }

        /* Prepare for next row */
        diff = largest_prime;   /* largest prime in this row (or 1 if none) */
        ++row_len;              /* next row will be one element longer */
    }
}
