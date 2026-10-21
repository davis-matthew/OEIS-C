/*
 * A213286  Number of 7-length words w over n-ary alphabet such that for every
 *          prefix z of w we have #(z,a_i) = 0 or #(z,a_i) >= #(z,a_j) for all
 *          j>i and #(z,a_i) counts the occurrences of the i-th letter in z.
 * https://oeis.org/A213286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(11954-29577*n+27640*n^2-12831*n^3+3234*n^4-420*n^5+24*n^6)/24
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 26, u64 n <= 567, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of 7‑letter words over an n‑ary alphabet satisfying the
   cumulative relative frequency property. */

seq_t a(idx_t n)
{
#if defined(__SIZEOF_INT128__)
    /* Use a 128‑bit signed integer for all intermediate arithmetic. */
    __int128 sn = (__int128)n;
    __int128 term = (__int128)24;           /* 24*/
    term = term * sn - (__int128)420;       /* 24n - 420*/
    term = term * sn + (__int128)3234;      /* 24n^2 - 420n + 3234*/
    term = term * sn - (__int128)12831;     /* 24n^3 - 420n^2 + 3234n - 12831*/
    term = term * sn + (__int128)27640;     /* 24n^4 - 420n^3 + 3234n^2 - 12831n + 27640*/
    term = term * sn - (__int128)29577;     /* 24n^5 - 420n^4 + 3234n^3 - 12831n^2 + 27640n - 29577*/
    term = term * sn + (__int128)11954;     /* 24n^6 - 420n^5 + 3234n^4 - 12831n^3 + 27640n^2 - 29577n + 11954*/
    __int128 result = sn * term / (__int128)24;  /* a(n) */
    return (seq_t)result;
#else
    /* Fallback implementation that works when __int128 is not available.
       It relies on the fact that the final result fits into seq_t; the
       intermediate values are computed using unsigned __int128 to avoid
       overflow. */
    unsigned __int128 sn = (unsigned __int128)n;
    unsigned __int128 term = (unsigned __int128)24;
    term = term * sn - (unsigned __int128)420;
    term = term * sn + (unsigned __int128)3234;
    term = term * sn - (unsigned __int128)12831;
    term = term * sn + (unsigned __int128)27640;
    term = term * sn - (unsigned __int128)29577;
    term = term * sn + (unsigned __int128)11954;
    unsigned __int128 result = sn * term / (unsigned __int128)24;
    return (seq_t)result;
#endif
}
