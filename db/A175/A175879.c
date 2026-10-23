/*
 * A175879  Numbers arising from certain regular binary expansions.
 * https://oeis.org/A175879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n, floor(n/k) odd} 2^(k-1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t val    = 1;          /* val = 2^(k-1) for current k */
    for (idx_t k = 1; k <= n; ++k) {
        if ( ((n / k) & 1) != 0 ) {   /* floor(n/k) is odd ? */
            result += val;
        }
        val *= 2;                  /* next power of two */
    }
    return result;
}
