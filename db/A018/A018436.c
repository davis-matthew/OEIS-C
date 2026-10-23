/*
 * A018436  Divisors of 405.
 * https://oeis.org/A018436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 405 in increasing order
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sorted list of divisors of 405 the first time the function
   is called, cache it, and return the nth element (1‑based).  If n is
   out of range, 0 is returned. */
#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* Cached divisor list and its length */
    static seq_t divisors[10];
    static size_t div_count = 0;

    if (div_count == 0) {
        /* Generate all divisors of 405 = 3^4 * 5^1 */
        const uint32_t pow3[5] = {1, 3, 9, 27, 81};
        const uint32_t pow5[2] = {1, 5};
        const size_t total   = 5 * 2;           /* 10 divisors */
        seq_t       temp[10];
        size_t      idx = 0;

        /* Enumerate all i,j combinations */
        for (size_t i = 0; i < 5; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                temp[idx++] = (seq_t)pow3[i] * (seq_t)pow5[j];
            }
        }

        /* Simple insertion sort into divisors (ascending order) */
        for (size_t i = 1; i < total; ++i) {
            seq_t key = temp[i];
            size_t j = i;
            while (j > 0 && temp[j - 1] > key) {
                temp[j] = temp[j - 1];
                --j;
            }
            temp[j] = key;
        }
        for (size_t i = 0; i < total; ++i) {
            divisors[i] = temp[i];
        }
        div_count = total;
    }

    if ((idx_t)n <= div_count) {
        return divisors[(size_t)(n - 1)];
    } else {
        return (seq_t)0;
    }
}
