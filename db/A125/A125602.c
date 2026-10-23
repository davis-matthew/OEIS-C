/*
 * A125602  Centered triangular numbers that are prime.
 * https://oeis.org/A125602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*m*(m-1)/2 + 1 for m ≥ 2, choosing only those values
 *            that are prime. The sequence is indexed from n = 1
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 68, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Undefined for indices below the offset 1; return 0 as a sentinel. */
    if (n == 0) return 0;

    idx_t count = 0;    /* how many prime centered triangular numbers found */
    idx_t m = 1;        /* current m in the formula (start with 1) */
    seq_t val = 0;      /* most recent prime value */

    while (count < n) {
        m++;  /* move to next candidate m */
        /* compute centered triangular number: 3*m*(m-1)/2 + 1 */
        val = ((seq_t)3 * (seq_t)m * (seq_t)(m - 1)) / (seq_t)2 + (seq_t)1;
        if (is_prime_big(val)) {
            count++; /* found another prime term */
        }
    }
    return val; /* nth prime centered triangular number */
}
