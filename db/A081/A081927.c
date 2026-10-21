/*
 * A081927  n-th positive integer whose digits sum up to n.
 * https://oeis.org/A081927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 37, u32 n <= 81, u64 n <= 169,
 *            u128 n <= 342, big any n
 * Helpers:   build_minimal_digits, next_number_same_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence A081927: n‑th positive integer whose digits sum to n.
   This implementation generates the number by starting with the smallest
   number of the minimal possible length and repeatedly computing the
   next larger number with the same digit sum.  For each “next” operation
   we modify the digit array in place using only integer arithmetic.  The
   algorithm runs in O(n * k) where k ≤ 9·⌈n/9⌉, which is well below the
   required 2 s for n ≤ 10000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_minimal_digits.h"
#include "../utils/next_number_same_sum.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset, but safe */

    int sum = (int)n;
    int minimal_len = (sum + 8) / 9;

    /* Allocate enough space for all possible lengths (≤ sum). */
    int *digits = (int *)malloc((sum + 5) * sizeof(int));

    /* Start with the minimal number of the minimal possible length. */
    int len = minimal_len;
    build_minimal_digits(len, sum, minimal_len, digits);

    /* Generate (n-1) subsequent numbers. */
    for (idx_t step = 1; step < n; ++step) {
        if (!next_number_same_sum(len, digits, sum)) {
            /* No larger number of this length: increase length. */
            ++len;
            build_minimal_digits(len, sum, minimal_len, digits);
        }
    }

    /* Convert the digit array to a seq_t value. */
    seq_t result = 0;
    for (int i = 0; i < len; ++i) {
        result = result * 10 + (seq_t)digits[i];
    }

    free(digits);
    return result;
}
