/*
 * A171870  For odd numbers x, a(x) is the number of complex numbers z in the
 *          zx + 1 problem giving the same number of iterations as the 3x + 1
 *          problem requires to reach 1.
 * https://oeis.org/A171870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd values encountered after the initial odd *
 *            in the standard Collatz trajectory for the odd start number
 *            2*n-1. * This equals A075680(n)-1
 * Range:     u8 n <= 13, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the odd start value: x = 2*n - 1 */
    seq_t start = (seq_t)n * 2 - 1;
    seq_t cur   = start;
    seq_t cnt   = 0;

    while (cur != 1)
    {
        if ((cur & 1) != 0)          /* cur is odd */
        {
            if (cur != start)
                cnt++;                /* count odd steps after the first */
            cur = cur * 3 + 1;        /* standard Collatz odd step */
        }
        else
        {
            cur >>= 1;                /* standard Collatz even step */
        }
    }
    return cnt;
}
