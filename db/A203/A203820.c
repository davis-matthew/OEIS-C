/*
 * A203820  Number of (n+1)X3 0..3 arrays with no 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1296 * 21^(n-1) (n ≥ 1)
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 we return 0, although the offset starts at 1. */
    if (n == 0)
        return 0;

    seq_t result = 1;
    idx_t times = n - 1;          /* exponent of 21 */

    /* Multiply repeatedly by the small base 21.  This keeps each
       intermediate multiplication simple (small × big), which is
       efficient for the arbitrary‑precision build and avoids
       the potentially expensive big * big multiplications of
       exponentiation‑by‑squaring. */
    for (idx_t i = 0; i < times; ++i)
        result *= (seq_t)21;

    result *= (seq_t)1296;        /* final scaling factor */
    return result;
}
