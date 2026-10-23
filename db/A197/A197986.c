/*
 * A197986  a(n) = round((n+1/n)^3).
 * https://oeis.org/A197986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((n+1/n)^3) = n*(n^2+3) for n > 6
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values for n = 1..6 are explicitly defined */
    if (n <= 6) {
        static const seq_t tbl[6] = {
            (seq_t)8ULL,  /* a(1) */
            (seq_t)16ULL, /* a(2) */
            (seq_t)37ULL, /* a(3) */
            (seq_t)77ULL, /* a(4) */
            (seq_t)141ULL,/* a(5) */
            (seq_t)235ULL /* a(6) */
        };
        return tbl[n-1];
    }

    /* for n ≥ 7 use the polynomial n·(n²+3)  */
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;          /* n²  */
    seq_t res = nn * (sq + (seq_t)3); /* n·(n²+3) */
    return res;
}
