/*
 * A036758  Number of edge-rooted tree-like octagonal systems.
 * https://oeis.org/A036758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A036758:
   For n ≥ 5,
   4*n*(2*n+1)*a(n) + 2*(-38*n^2+59*n-24)*a(n-1)
   + 42*(-4*n^2+4*n+7)*a(n-2)
   + 98*(13*n-31)*(n-3)*a(n-3)
   - 343*(n-3)*(n-4)*a(n-4) = 0
   Base values:
   a(1)=1, a(2)=5, a(3)=31, a(4)=216. */

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;   /* undefined below offset, return 0 */
    }
    /* Handle base cases directly */
    if (n <= 4) {
        static const seq_t base[5] = {0, 1, 5, 31, 216};
        return base[n];
    }

    /* Use a sliding window of the last four values: a(n-4)..a(n-1) */
    seq_t a1 = 1;   /* a(1) */
    seq_t a2 = 5;   /* a(2) */
    seq_t a3 = 31;  /* a(3) */
    seq_t a4 = 216; /* a(4) */

    for (idx_t k = 5; k <= n; ++k) {
        idx_t nk = k;                                     /* current index */
        /* Coefficients derived from the recurrence */
        idx_t coeff1 = (idx_t)2 * ((idx_t)38 * nk * nk
                                   - (idx_t)59 * nk
                                   + (idx_t)24);

        idx_t coeff2 = (idx_t)42 * ((idx_t)4 * nk * nk
                                   - (idx_t)4 * nk
                                   - (idx_t)7);

        idx_t coeff3 = (idx_t)98 * ((idx_t)13 * nk
                                   - (idx_t)31) * (nk - (idx_t)3);

        idx_t coeff4 = (idx_t)343 * (nk - (idx_t)3) * (nk - (idx_t)4);

        /* Compute terms in 128‑bit to avoid overflow on intermediate results */
        unsigned __int128 term1 = (unsigned __int128)coeff1
                                  * (unsigned __int128)a4;

        unsigned __int128 term2 = (unsigned __int128)coeff2
                                  * (unsigned __int128)a3;

        unsigned __int128 term3 = (unsigned __int128)coeff3
                                  * (unsigned __int128)a2;

        unsigned __int128 term4 = (unsigned __int128)coeff4
                                  * (unsigned __int128)a1;

        unsigned __int128 num = term1 + term2 + term4 - term3;

        idx_t denom = (idx_t)4 * nk * (2 * nk + 1);
        seq_t result = (seq_t)(num / denom);

        /* Slide the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = result;
    }

    return a4;
}
