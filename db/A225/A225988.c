/*
 * A225988  Number of pairs of orthogonal (-1,2) vectors of length 3*n.
 * https://oeis.org/A225988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{n-1} * sum_{k=0}^{n} C(3n, 2n-2k) * C(n+2k, n+k) * 2^k
 *            where C(a,b) is the binomial coefficient a choose b
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   binom, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* the offset is 1, n=0 is not used */

    /* initial combinatorial value M_0 = C(3n, 2n) = C(3n, n) */
    seq_t M = binom(3*n, n);

    seq_t sum = 0;
    seq_t pow2_k = 1;              /* 2^k for the current k */
    idx_t a_val = 2*n;             /* a = 2n - 2k   */
    idx_t b_val = n;               /* b = n + k     */
    idx_t c_val = 0;               /* c = k         */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* add term M * 2^k to the sum */
        sum += M * pow2_k;

        if (k == n) break;          /* last iteration, nothing more to do */

        /* update M to M_{k+1} = M_k * a*(a-1) / ((b+1)*(c+1))  */
        seq_t numerator   = (seq_t)a_val * (seq_t)(a_val - 1);
        seq_t denominator = (seq_t)(b_val + 1) * (seq_t)(c_val + 1);
        M = (M * numerator) / denominator;

        /* next power of two (2^{k+1}) */
        pow2_k = pow2_k * 2;

        /* advance indices for the next k */
        a_val -= 2;
        b_val += 1;
        c_val += 1;
    }

    /* final multiplication by 2^{n-1} to obtain the result */
    seq_t pow2_final = pow_seq(2, (int)(n - 1));
    return sum * pow2_final;
}
