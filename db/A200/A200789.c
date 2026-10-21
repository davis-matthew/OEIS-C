/*
 * A200789  Number of 0..n arrays x(0..6) of 7 elements without any two
 *          consecutive increases.
 * https://oeis.org/A200789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5040 + 40704*n + 127582*n^2 + 199213*n^3 + 169960*n^4 +
 *            80626*n^5 + 19978*n^6 + 2017*n^7) / 5040
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 25, u64 n <= 137, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values a(1) … a(8) */
    const seq_t init[9] = {
        0,          /* a(0) unused */
        128ULL,     /* a(1) */
        1791ULL,    /* a(2) */
        11704ULL,   /* a(3) */
        50775ULL,   /* a(4) */
        169884ULL,  /* a(5) */
        474566ULL,  /* a(6) */
        1160616ULL, /* a(7) */
        2562633ULL  /* a(8) */
    };

    if (n == 0)
        return 0;          /* offset is 1, but we guard against 0 just in case */

    if (n <= 8)
        return init[n];

    /* Use 128‑bit intermediate arithmetic so the calculation stays exact
       even when the final result does not fit in a 64‑bit integer. */
    __int128 N  = n;
    __int128 n1 = N;          /* n           */
    __int128 n2 = n1 * n1;    /* n^2         */
    __int128 n3 = n2 * n1;    /* n^3         */
    __int128 n4 = n3 * n1;    /* n^4         */
    __int128 n5 = n4 * n1;    /* n^5         */
    __int128 n6 = n5 * n1;    /* n^6         */
    __int128 n7 = n6 * n1;    /* n^7         */

    __int128 num = 5040
                 + 40704 * n1
                 + 127582 * n2
                 + 199213 * n3
                 + 169960 * n4
                 + 80626  * n5
                 + 19978  * n6
                 + 2017   * n7;

    return (seq_t)(num / 5040);
}
