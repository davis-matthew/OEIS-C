/*
 * A088829  Even numbers with even abundance.
 * https://oeis.org/A088829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088829: a(n) = the n-th even integer whose sum of divisors is even
 * (i.e., the odd part of n is not a perfect square). */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1, just for safety */
    idx_t count = 0;
    seq_t cand   = 6;              /* first even number in the sequence */

    while (count < n) {
        seq_t m = cand;
        /* strip all factors of 2 */
        while ((m % 2) == 0) m /= 2;
        seq_t s = floor_sqrt(m);  /* integer square root of the odd part */
        if (s * s != m) {         /* odd part not a perfect square -> keep */
            ++count;
            if (count == n) return cand;
        }
        cand += 2;                 /* next even integer */
    }
    return 0; /* unreachable */
}
