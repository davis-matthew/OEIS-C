/*
 * A065408  Squares whose digits sum to a prime.
 * https://oeis.org/A065408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 74, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum_seq, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: squares whose digits sum to a prime.
   a(n) = (k^2) where k is the n-th positive integer
   such that the decimal digit sum of k^2 is a prime.
   Computation is performed by brute‑force search. */

#include <stdbool.h>
#include "../utils/digit_sum_seq.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;              /* undefined for n < 1, return 0 as placeholder */
    idx_t count = 0;                   /* how many valid squares found so far */
    idx_t k = 0;                       /* current integer to test */
    seq_t result = 0;

    while (count < n) {
        ++k;
        seq_t sq = (seq_t)k * (seq_t)k;    /* square */
        seq_t sum = digit_sum_seq(sq);    /* digit sum of the square */
        if (is_prime_small(sum)) {         /* check if sum is prime */
            ++count;
            if (count == n) {
                result = sq;
                break;
            }
        }
    }
    return result;
}
