/*
 * A210376  Number of 2 X 2 matrices with all terms in {0,1,...,n} and (sum of
 *          terms) = n + 4.
 * https://oeis.org/A210376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 18*n^2 + 107*n - 270) / 6 for n ≥ 3; a(0) = 0,
 *            a(1) = 0, a(2) = 10
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the exceptional small indices explicitly. */
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 10;

    /* For n ≥ 3 use the closed‑form formula.
       Use a larger integer type for the intermediate calculation
       to avoid overflow on builds where seq_t is smaller than 64 bits. */
    unsigned __int128 x = (unsigned __int128)n;            /* x = n */
    unsigned __int128 xx = x * x;                         /* x² */
    unsigned __int128 xxx = xx * x;                       /* x³ */
    unsigned __int128 num = xxx + (unsigned __int128)18 * xx
                           + (unsigned __int128)107 * x - 270;
    unsigned __int128 val = num / 6;                      /* division by 6 is exact */

    return (seq_t)val;
}
