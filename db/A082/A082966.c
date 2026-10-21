/*
 * A082966  Number of inequivalent ways (mod D_4) three checkers can be placed
 *          on an n X n board.
 * https://oeis.org/A082966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/48)*(n-1)*(n^5 + n^4 - 2*n^3 + 14*n^2 - 5*n + 3) if n
 *            is odd; a(n) = (1/48)*n*(n-1)*(n^2 - n + 2)*(n^2 + 2*n - 2) if n
 *            is even. a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 76, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The result is always an integer; we compute in 128‑bit to avoid
       intermediate overflow and then cast back to seq_t. */
    if (n <= 1)
        return 0;

    unsigned __int128 res;
    if (n & 1) {   /* odd n */
        unsigned __int128 nn = n;
        unsigned __int128 poly = nn * nn * nn * nn * nn;      /* n^5 */
        poly += nn * nn * nn * nn;                           /* + n^4 */
        poly -= 2 * nn * nn * nn;                            /* - 2 n^3 */
        poly += 14 * nn * nn;                                /* + 14 n^2 */
        poly -= 5 * nn;                                      /* - 5 n */
        poly += 3;                                           /* + 3 */
        res = (nn - 1) * poly;
    } else {        /* even n */
        unsigned __int128 nn = n;
        unsigned __int128 part1 = nn * (nn - 1);                                   /* n*(n-1) */
        unsigned __int128 part2 = nn * nn - nn + 2;                               /* n^2 - n + 2 */
        unsigned __int128 part3 = nn * nn + 2 * nn - 2;                           /* n^2 + 2 n - 2 */
        res = part1 * part2 * part3;
    }

    res /= 48;
    return (seq_t)res;
}
