/*
 * A001324  Number of equivalence classes of n-valued Post functions of 2
 *          variables under action of semi-direct product of symmetric group
 *          S_2 and complementing group C(2,n).
 * https://oeis.org/A001324
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside count over the group S_2 x| (Z_n)^2 of order 2n^2
 *            acting on {0..n-1}^2: a(n) = (1/(2n^2)) * ( Sum_{d|n} J_2(d) *
 *            n^(n^2/d) + n * Sum_{m|n} phi(m) * n^((n^2 + n*[m odd])/(2m)) ),
 *            where J_2 is Jordan's totient (number of elements of order d in
 *            Z_n^2) and phi is Euler's totient. Each term i...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   p1324_add, p1324_jordan, p1324_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/p1324_add.h"
#include "../utils/p1324_jordan.h"
#include "../utils/p1324_pow.h"

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t n2 = N * N;
    seq_t S = 0;
    int carry = 0;
    uint64_t d;
    seq_t res;

    for (d = 1; d <= N; d++) {
        if (N % d != 0) continue;
        /* translations of order d: n^2/d cycles; divide by n^2 */
        {
            uint64_t e = n2 / d - 2;
            seq_t t = (seq_t)p1324_jordan(d, 2) * p1324_pow((seq_t)N, e);
            p1324_add(&S, &carry, t);
        }
        /* swap-type elements with translation-of-square order m = d */
        {
            uint64_t m = d;
            uint64_t cyc = (m & 1) ? (n2 + N) / (2 * m) : n2 / (2 * m);
            /* n * n^cyc / n^2 = n^(cyc - 1) */
            seq_t t = (seq_t)p1324_jordan(m, 1) * p1324_pow((seq_t)N, cyc - 1);
            p1324_add(&S, &carry, t);
        }
    }

    res = S / 2;
    if (carry) {
        seq_t half = ((seq_t)0 - 1) / 2 + 1;
        res += half * (seq_t)carry;
    }
    return res;
}
