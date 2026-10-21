/*
 * A076998  Difference between cubefree and squarefree components of n.
 * https://oeis.org/A076998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A007948(n) - A007947(n) Difference between cubefree and
 *            squarefree parts of n (A007948 – largest cubefree divisor,
 *            A007947 – squarefree part)
 * Range:     u8 n <= 288, u16 n <= 9812, u32 n <= 9812, u64 n <= 9812,
 *            u128 n <= 9812, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    idx_t tmp = n;
    seq_t cubefree = 1;
    seq_t squarefree = 1;

    /* handle factor 2 */
    if ((tmp & 1ULL) == 0) {
        unsigned e = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            ++e;
        }
        squarefree *= (seq_t)2;
        cubefree *= (e >= 2) ? (seq_t)4 : (seq_t)2;
    }

    /* odd factors */
    for (idx_t p = 3; (seq_t)p * (seq_t)p <= tmp; p += 2) {
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            squarefree *= (seq_t)p;
            cubefree *= (e >= 2) ? ((seq_t)p * (seq_t)p) : (seq_t)p;
        }
    }

    /* remaining prime factor > sqrt(tmp) */
    if (tmp > 1) {
        squarefree *= (seq_t)tmp;
        cubefree *= (seq_t)tmp;
    }

    return cubefree - squarefree;
}
