/*
 * A072866  Smallest k > 1 dividing tau(k*2^n) (cf. A000005).
 * https://oeis.org/A072866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k>1 such that k divides tau(k*2^n)
 * Range:     u8 n <= 9344, u16 n <= 9344, u32 n <= 9344, u64 n <= 9344,
 *            u128 n <= 9344, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A072866 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Pre‑computed values: number of twos in k (v2) and product of (ei+1)
       over odd prime factors of k (C). Only k = 2 … 12 are needed. */
    static const int v2[13] = {
        0, /* dummy for 0 */
        0, /* 1 unused */
        1, /* 2 */
        0, /* 3 */
        2, /* 4 */
        0, /* 5 */
        1, /* 6 */
        0, /* 7 */
        3, /* 8 */
        0, /* 9 */
        1, /* 10 */
        0, /* 11 */
        2  /* 12 */
    };
    static const int C[13] = {
        0, /* dummy for 0 */
        0, /* 1 unused */
        1, /* 2 */
        2, /* 3 */
        1, /* 4 */
        2, /* 5 */
        2, /* 6 */
        2, /* 7 */
        1, /* 8 */
        3, /* 9 */
        2, /* 10 */
        2, /* 11 */
        2  /* 12 */
    };

    /* Iterate k from 2 to 12 and test the divisibility condition.
       Use modular arithmetic to avoid overflow on small seq_t types. */
    for (int k = 2; k <= 12; ++k) {
        /* Compute tau(k*2^n) mod k:
           tau = (n + v2[k] + 1) * C[k]
           Since we only need the residue modulo k, we reduce
           intermediate results modulo k. */
        unsigned long long part = ((unsigned long long)n + v2[k] + 1) % k;
        part = (part * C[k]) % k;

        if (part == 0) {
            return (seq_t)k;
        }
    }
    /* According to the definition, one of k=2..12 will always satisfy the
       condition. Return 0 as a safety guard (should never happen). */
    return (seq_t)0;
}
