/*
 * A210427  Number of semistandard Young tableaux over all partitions of 5
 *          with maximal element <= n.
 * https://oeis.org/A210427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(12+(35+13*n^2)*n^2)/60
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 114, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;                 /* n */
    seq_t a2  = nn * nn;                  /* n^2 */
    seq_t inner = 12 + (35 + 13 * a2) * a2; /* 12 + (35+13*n^2)*n^2 */

    /* use 128‑bit intermediate to avoid overflow on 64‑bit builds */
    unsigned __int128 prod = (unsigned __int128)nn * (unsigned __int128)inner;
    return (seq_t)(prod / 60);
}
