/*
 * A126241  Dropping time of n in the 3n+1 problem (or the Collatz problem).
 *          Let T(n):=n/2 if n is even, (3n+1)/2 otherwise (A014682). Let a(n)
 *          be the smallest integer k such that T^(k)(n)<n, where T^(k) is the
 *          k-th iterate, or infinity otherwise; a(n) is called the dropping
 *          time of n.
 * https://oeis.org/A126241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 7486, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the dropping time a(n) for the sequence A126241.
 * a(n) = smallest k such that T^k(n) < n, where
 *        T(n) = n/2 if n is even, (3n+1)/2 if n is odd.
 * Implementation uses a direct iterative search.
 */
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n <= 1) return 0;              /* a(1) = 0 by definition */
    seq_t target = (seq_t)n;
    seq_t x = target;
    seq_t k = 0;

    while (1) {
        k++;
        if (x & 1) {                    /* odd */
            x = (x * (seq_t)3 + (seq_t)1) / (seq_t)2;
        } else {                        /* even */
            x = x / (seq_t)2;
        }
        if (x < target) break;
    }
    return k;
}
