/*
 * A107550  Coordination sequence for B_17 lattice.
 * https://oeis.org/A107550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n) - F(n-1) where * F(n) = Σ_{k=0}^{17} C(17,k) · 2^k ·
 *            G(k,n) * G(k,n) = 1 (k = 0) * = C(2n,k) – k·C(n,k) (k ≤ n) * =
 *            C(2n,k) (k > n) * * The sequence counts the number of lattice
 *            points at graph distance n * in the B_17 lattice
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    const int DIM = 17;
    seq_t F_n  = 0;
    seq_t F_nm = 0;
    idx_t n1   = n - 1;

    for (int k = 0; k <= DIM; ++k)
    {
        seq_t C17k  = binom(17, k);
        seq_t pow2k = (seq_t)(1ULL << k);

        /* ---- term for n ---- */
        seq_t Gkn;
        if (k == 0)
            Gkn = 1;
        else if ((idx_t)k <= n)
        {
            seq_t c2n_k = binom(2 * n, k);
            seq_t cn_k  = binom(n, k);
            Gkn = c2n_k - cn_k * (seq_t)k;
        }
        else
            Gkn = binom(2 * n, k);

        F_n += C17k * pow2k * Gkn;

        /* ---- term for n-1 ---- */
        seq_t Gkn1;
        if (k == 0)
            Gkn1 = 1;
        else if ((idx_t)k <= n1)
        {
            seq_t c2n1_k = binom(2 * n1, k);
            seq_t cn1_k  = binom(n1, k);
            Gkn1 = c2n1_k - cn1_k * (seq_t)k;
        }
        else
            Gkn1 = binom(2 * n1, k);

        F_nm += C17k * pow2k * Gkn1;
    }

    return F_n - F_nm;
}
