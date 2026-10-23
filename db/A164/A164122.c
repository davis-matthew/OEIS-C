/*
 * A164122  a(n) = A164121(n+1)*2 - A164121(n).
 * https://oeis.org/A164122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*b(n+1) - b(n) * where b(1) = 1 and for n ≥ 1, b(n+1) is
 *            the smallest odd integer * > b(n) such that 2*b(n+1) - b(n) is
 *            prime. a(n) is the resulting * prime. The sequence is the list
 *            of these primes with repetitions * as defined by the
 *            construction
 * Range:     u8 n <= 60, u16 n <= 9144, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n <= 1) {            /* a(1) = 5 directly */
        return (seq_t)5;
    }

    /* Current values of the b‑sequence */
    seq_t b_curr  = (seq_t)1;    /* b(1)   */
    seq_t b_next  = (seq_t)3;    /* b(2)   */

    /* Iterate until the desired index is reached */
    for (idx_t i = 2; i <= n; ++i) {
        /* Find the smallest odd candidate > b_next */
        uint64_t cand_b = (uint64_t)b_next + 2;          /* next odd */
        seq_t  cand_a;
        while (1) {
            cand_a = (seq_t)((cand_b * 2ULL) - b_next); /* 2*cand_b - b_next */
            if (is_prime_small(cand_a)) {
                break;
            }
            cand_b += 2ULL;                               /* keep on odds */
        }

        /* Update for next iteration */
        b_curr = b_next;
        b_next = (seq_t)cand_b;

        if (i == n) {
            return cand_a;
        }
    }

    /* Should never reach here but keep the compiler happy */
    return (seq_t)0;
}
