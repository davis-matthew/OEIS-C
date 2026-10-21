/*
 * A060480  Number of orbits of length n in map whose periodic points come
 *          from A059990.
 * https://oeis.org/A060480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * a(n) = (1/n) * Σ_{d|n} μ(d) * b(n/d) * where b(k) = 2^k - 1 (k
 *            odd) * b(k) = (2^k - 1) / 3^{1+v3(k)} (k even) * with v3(k) the
 *            exponent of 3 in k. This follows from the * description of
 *            A059990, and allows us to compute a(n) * without hard‑coding
 *            values
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   mu_of, pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mu_of.h"
#include "../utils/pow2.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    seq_t posSum = 0;      /* Σ μ(d)=1 terms * b(n/d)             */
    seq_t negSum = 0;      /* Σ μ(d)=-1 terms * b(n/d)            */
    /* enumerate all divisors d of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        /* process divisor d */
        int mu_d = mu_of(d);
        idx_t k = n / d;
        /* compute b(k) */
        seq_t pow2k = pow2(k);          /* 2^k  */
        seq_t bk = pow2k - (seq_t)1;    /* 2^k - 1 */

        if ((k & 1) == 0) {           /* k is even */
            /* exponent of 3 in k */
            int v3 = 0;
            idx_t t = k;
            while (t % 3 == 0) { v3++; t /= 3; }
            seq_t pow3 = pow_seq((seq_t)3, 1 + v3);  /* 3^(1+v3) */
            bk /= pow3;                 /* exact division */
        }

        if (mu_d > 0) posSum += bk;
        else if (mu_d < 0) negSum += bk;
        /* mu_d cannot be 0 when d is squarefree, otherwise 0 */

        /* process paired divisor n/d if different */
        idx_t d2 = n / d;
        if (d2 != d) {
            int mu_d2 = mu_of(d2);
            idx_t k2 = n / d2;        /* equals d */
            seq_t pow2k2 = pow2(k2);
            seq_t bk2 = pow2k2 - (seq_t)1;
            if ((k2 & 1) == 0) {
                int v3 = 0; idx_t t2 = k2;
                while (t2 % 3 == 0) { v3++; t2 /= 3; }
                seq_t pow3 = pow_seq((seq_t)3, 1 + v3);
                bk2 /= pow3;
            }
            if (mu_d2 > 0) posSum += bk2;
            else if (mu_d2 < 0) negSum += bk2;
        }
    }

    seq_t numerator = posSum - negSum;       /* guaranteed non‑negative */
    return numerator / (seq_t)n;
}
