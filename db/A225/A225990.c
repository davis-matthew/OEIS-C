/*
 * A225990  Number of pairs of orthogonal (-2,3) vectors of length 5*n.
 * https://oeis.org/A225990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Σ_{t=0}^{n} C(5n,3(n‑t)) * C(2n+3t,2t) * 2^{2n+t}
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   binom, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts unordered pairs of orthogonal (-2,3) vectors of length 5*n. */

#include "../utils/binom.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* a(0) is not defined in OEIS; return 0. */

    idx_t five_n = 5 * n;
    seq_t total = 0;

    for (idx_t t = 0; t <= n; ++t) {
        idx_t i = 3 * (n - t);                 /* choose i = 3(n‑t) positions for +4 */
        idx_t k = 2 * t;                       /* choose k = 2t positions for +9 */
        idx_t m = 2 * n + 3 * t;               /* remaining positions count */

        /* C(5n, i)   and   C(m, k)   are binomial coefficients. */
        seq_t c1   = binom(five_n, i);
        seq_t c2   = binom(m, k);

        /* 2^{2n+t}  multiplication. */
        seq_t pow2 = pow_seq((seq_t)2, (int)(2 * n + t));

        seq_t term = c1;
        term = term * c2;
        term = term * pow2;

        total += term;
    }

    /* Unordered pairs: divide by 2.  The exact division is safe because every
       term in the sum is even (2^{2n+t} guarantees a factor of 2). */
    total = total / (seq_t)2;
    return total;
}
