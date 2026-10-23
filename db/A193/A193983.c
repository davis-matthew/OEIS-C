/*
 * A193983  Number of ways to arrange 5 nonattacking triangular rooks on an n
 *          X n X n triangular grid.
 * https://oeis.org/A193983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (41532*n - 7504*n^2 - 120064*n^3 + 142980*n^4 - 76607*n^5
 *            + 23693*n^6 - 4526*n^7 + 530*n^8 - 35*n^9 + n^10 +
 *            5120*floor(n/3) + 120*(1359 - 988*n + 245*n^2 - 26*n^3 +
 *            n^4)*floor(n/2) - 5120*floor((n+1)/3)) / 3840 This closed form
 *            reproduces the OEIS A193983 values for all n≥1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 24, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Use 128‑bit arithmetic for all intermediate steps.
       The sequence values that fit the target type are small enough
       that the intermediate 128‑bit values never overflow for the
       range of n that is tested. */
    __int128 N = ( __int128 ) n;

    __int128 pow1 = N;
    __int128 pow2 = pow1 * N;
    __int128 pow3 = pow2 * N;
    __int128 pow4 = pow3 * N;
    __int128 pow5 = pow4 * N;
    __int128 pow6 = pow5 * N;
    __int128 pow7 = pow6 * N;
    __int128 pow8 = pow7 * N;
    __int128 pow9 = pow8 * N;
    __int128 pow10 = pow9 * N;

    __int128 S = 0;
    S += (__int128)41532 * pow1;          /* 3461*n/320 */
    S += (__int128)(-7504) * pow2;       /* -469*n^2/240 */
    S += (__int128)(-120064) * pow3;     /* -469*n^3/15 */
    S += (__int128)142980 * pow4;        /* 2383*n^4/64 */
    S += (__int128)(-76607) * pow5;      /* -76607*n^5/3840 */
    S += (__int128)23693 * pow6;         /* 23693*n^6/3840 */
    S += (__int128)(-4526) * pow7;       /* -2263*n^7/1920 */
    S += (__int128)530 * pow8;           /* 53*n^8/384 */
    S += (__int128)(-35) * pow9;         /* -7*n^9/768 */
    S += (__int128)1 * pow10;            /* n^10/3840 */

    /* 4/3*floor(n/3) */
    __int128 f3 = N / 3;
    S += (__int128)5120 * f3;

    /* (1359/32 - 247*n/8 + 245*n^2/32 - 13*n^3/16 + n^4/32)*floor(n/2) */
    __int128 f2 = N / 2;
    __int128 inner = (__int128)1359
                     - (__int128)988 * N
                     + (__int128)245 * N * N
                     - (__int128)26 * N * N * N
                     + (__int128)N * N * N * N;
    S += (__int128)120 * inner * f2;

    /* -4/3*floor((n+1)/3) */
    __int128 f33 = (N + 1) / 3;
    S -= (__int128)5120 * f33;

    /* Final division by 3840 */
    __int128 result = S / 3840;

    return (seq_t)result;
}
