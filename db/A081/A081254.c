/*
 * A081254  Numbers k such that A081252(m)/m^2 has a local maximum for m = k.
 * https://oeis.org/A081254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*a(n-1) + (1+(-1)^n)/2, a(1) = 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */
    seq_t val = 1;                /* a(1) */
    if (n == 1) return val;
    for (idx_t i = 2; i <= n; ++i) {
        val = val + val;          /* val = 2*val */
        if ((i & 1ULL) == 0)     /* even index: (1+(-1)^i)/2 = 1 */
            val = val + 1;
        /* odd index: add 0, so nothing needed */
    }
    return val;
}
