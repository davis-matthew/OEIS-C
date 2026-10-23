/*
 * A178464  Minimal number of lines that must be removed from an n X n square
 *          array of cells in order to break all rectangles.
 * https://oeis.org/A178464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(2*n^2/3) + (n%2==1 || n%3==0 ? 1 : 0), with a(1)=1
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 1;
    /* compute ceil(2*n^2/3) */
    seq_t nn = (seq_t)n * (seq_t)n;          /* n^2               */
    seq_t base = (nn * 2 + 2) / 3;           /* (2*n^2 + 2)/3  → ceil(2*n^2/3) */
    seq_t add  = ((n & 1) || (n % 3 == 0)) ? 1 : 0;
    return base + add;
}
