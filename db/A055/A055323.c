/*
 * A055323  Number of labeled trees with n nodes and 11 leaves.
 * https://oeis.org/A055323
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! *
 *            (n-11)^2*(n-10)^2*(n-9)*(n-8)*(n-7)*(n-6)*(n-5)*(n-4)*(n-3)*(n-2)
 *            * (15*n^6 - 825*n^5 + 18615*n^4 - 220127*n^3 + 1435578*n^2 -
 *            4882792*n + 6746112) / 111244864389120000
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 14, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Values below the offset are undefined – return 0 for safety */
    if (n < 12) return 0;

    const uint64_t DEN = 111244864389120000ULL;

    /* ------------------------------------------------------------
       64‑bit implementation (to guard against overflow).
       ------------------------------------------------------------ */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 num = 1;
        /* factorial part */
        for (idx_t i = 2; i <= n; ++i) num *= (unsigned __int128)i;

        unsigned __int128 nn = (unsigned __int128)n;
        unsigned __int128 term;

        term = nn - 11;  /* (n‑11)^2 */
        num *= term * term;
        term = nn - 10;  /* (n‑10)^2 */
        num *= term * term;

        for (int offset = 9; offset >= 2; --offset)
            num *= (nn - offset);   /* (n‑9)…(n‑2) */

        /* polynomial part */
        int64_t coeff[7] = {6746112LL, -4882792LL, 1435578LL,
                            -220127LL, 18615LL, -825LL, 15LL};
        unsigned __int128 pow = 1;
        unsigned __int128 poly = 0;
        for (int i = 0; i < 7; ++i) {
            int64_t c = coeff[i];
            unsigned __int128 t = pow * (c >= 0 ? (unsigned __int128)c
                                               : (unsigned __int128)(-c));
            if (c >= 0) poly += t;
            else        poly -= t;
            pow *= nn;                /* advance to next power of n */
        }

        num *= poly;                  /* multiply by polynomial */
        unsigned __int128 res = num / (unsigned __int128)DEN;
        return (seq_t)res;
    }

    /* ------------------------------------------------------------
       Arbitrary‑precision implementation (seq_t may be a big
       integer type – operations behave as for normal integers).
       ------------------------------------------------------------ */
    else {
        seq_t num = 1;
        for (idx_t i = 2; i <= n; ++i) num *= (seq_t)i;

        seq_t nn = (seq_t)n;
        seq_t term;

        term = nn - 11;  /* (n‑11)^2 */
        num *= term * term;
        term = nn - 10;  /* (n‑10)^2 */
        num *= term * term;

        for (int offset = 9; offset >= 2; --offset)
            num *= (nn - offset);   /* (n‑9)…(n‑2) */

        /* polynomial part */
        const int64_t coeff[7] = {6746112LL, -4882792LL, 1435578LL,
                                  -220127LL, 18615LL, -825LL, 15LL};
        seq_t pow = 1;
        seq_t poly = 0;
        for (int i = 0; i < 7; ++i) {
            int64_t c = coeff[i];
            seq_t t = pow * (c >= 0 ? (seq_t)c : (seq_t)(-c));
            if (c >= 0) poly += t;
            else        poly -= t;
            pow *= nn;
        }

        num *= poly;
        seq_t res = num / (seq_t)DEN;
        return res;
    }
}
