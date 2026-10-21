/*
 * A228258  Number of third differences of arrays of length n+3 of numbers in
 *          0..6.
 * https://oeis.org/A228258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 42*a(n-2) for n>12 Initial values (n=1..12):
 *            a(1)=49, a(2)=1267, a(3)=22727, a(4)=310425, a(5)=3298681,
 *            a(6)=28791007, a(7)=218845881, a(8)=1609259287,
 *            a(9)=11658284065, a(10)=83824687591, a(11)=599858908753,
 *            a(12)=4277376525367
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* array of the first 12 terms, 0-indexed: init[0] = a(1) */
    static const seq_t init[12] = {
        49ULL,
        1267ULL,
        22727ULL,
        310425ULL,
        3298681ULL,
        28791007ULL,
        218845881ULL,
        1609259287ULL,
        11658284065ULL,
        83824687591ULL,
        599858908753ULL,
        4277376525367ULL
    };

    if (n <= 12) {
        return init[n-1];
    }

    /* start recurrence from a(11) and a(12) */
    seq_t a_nm2 = init[10]; /* a(11) */
    seq_t a_nm1 = init[11]; /* a(12) */
    seq_t a_n = a_nm1;      /* placeholder */

    for (idx_t i = 13; i <= n; ++i) {
        a_n = 13 * a_nm1 - 42 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
