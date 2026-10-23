/*
 * A187593  Number of 10-step E, S, NW and NE-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 108852*n^2 - 978404*n + 2100276 for n > 8 * For n <= 8
 *            the values are given explicitly: * a(1) = 0, a(2) = 0, a(3) = 0,
 *            a(4) = 3108, a(5) = 45960, * a(6) = 219432, a(7) = 609070, a(8)
 *            = 1243461
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed first 8 terms (index 0 unused) */
    static const seq_t tbl[9] = {
        0ULL,   /* a(0) unused */
        0ULL,   /* a(1) */
        0ULL,   /* a(2) */
        0ULL,   /* a(3) */
        3108ULL,      /* a(4) */
        45960ULL,     /* a(5) */
        219432ULL,    /* a(6) */
        609070ULL,    /* a(7) */
        1243461ULL    /* a(8) */
    };

    if (n <= 8)
        return tbl[n];

    seq_t nn = (seq_t)n;
    return 108852ULL * nn * nn - 978404ULL * nn + 2100276ULL;
}
