/*
 * A038459  A sequence for measuring seconds: read it aloud.
 * https://oeis.org/A038459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n-1)=n, a(2n)=1000
 * Range:     u8 n <= 1, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based.  Even indices return 1000; odd indices return (n+1)/2. */
    if ((n & 1ULL) == 0ULL) {
        return (seq_t)1000;
    } else {
        return (seq_t)((n + 1ULL) / 2ULL);
    }
}
