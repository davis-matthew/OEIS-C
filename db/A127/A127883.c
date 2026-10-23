/*
 * A127883  a(n) = 60*(n^5/120 + n^4/24 + n^3/6 + n^2/2 + n + 1).
 * https://oeis.org/A127883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^5 + 5 n^4 + 20 n^3 + 60 n^2 + 120 n + 120) / 2
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 95, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the nth term of OEIS A127883. */
    if (n == 0)
        return 0; /* out of range, but harmless */

    unsigned __int128 nn = n;          /* 128‑bit intermediate */
    unsigned __int128 term =
        (nn * nn * nn * nn * nn)                /* n^5 */
        + 5ULL * (nn * nn * nn * nn)           /* 5 n^4 */
        + 20ULL * (nn * nn * nn)               /* 20 n^3 */
        + 60ULL * (nn * nn)                    /* 60 n^2 */
        + 120ULL * nn                          /* 120 n */
        + 120ULL;                              /* +120 */

    /* term is even; shift right by 1 to divide by 2. */
    return (seq_t)(term >> 1);
}
