/*
 * A192658  Binomial convolution of the Floor-Sqrt transform of central
 *          binomial coefficients.
 * https://oeis.org/A192658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * floor(sqrt(binomial(2k,k)))
 *            * * * floor(sqrt(binomial(2n-2k,n-k)))
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Pre‑compute the central binomial coefficient for m = n  */
    seq_t c2n_n = 1;                 /* C(2*0,0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        /* C(2*i,i) = C(2*(i-1),(i-1)) * (4*i-2)/i */
        c2n_n = c2n_n * ((seq_t)(4 * i - 2)) / (seq_t)i;
    }

    seq_t sum = 0;
    seq_t bin_nk = 1;                /* C(n,0) */
    seq_t c2k_k = 1;                 /* C(0,0) = 1 */
    seq_t c2m_m = c2n_n;             /* C(2*n,n) for m = n-k */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t fs_k   = floor_sqrt(c2k_k);
        seq_t fs_m   = floor_sqrt(c2m_m);
        seq_t term   = bin_nk * fs_k;
        term        *= fs_m;
        sum         += term;

        if (k == n) break;          /* last iteration */

        /* Update bin_nk   :   C(n,k+1)  = C(n,k) * (n-k)/(k+1) */
        bin_nk = bin_nk * (seq_t)(n - k) / (seq_t)(k + 1);

        /* Update c2k_k   :   C(2*(k+1),k+1) = C(2*k,k) * (4*(k+1)-2)/(k+1) */
        idx_t nextk = k + 1;
        c2k_k = c2k_k * (seq_t)(4 * nextk - 2) / (seq_t)nextk;

        /* Update c2m_m   :   let m = n-k;   C(2*(m-1),m-1) = C(2*m,m) * m/(4*m-2) */
        idx_t m = n - k;
        c2m_m = c2m_m * (seq_t)m / (seq_t)(4 * m - 2);
    }

    return sum;
}
