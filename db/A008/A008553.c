/*
 * A008553  Numbers whose American English name contains the letter 'y'.
 * https://oeis.org/A008553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = 100 * floor((n-1)/80) + 20 + 10 * floor(((n-1) % 80) /
 *            10) + ((n-1) % 10)
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not used, offset is 1, but safety */
    idx_t m = n - 1;
    idx_t full   = m / 80;          /* number of complete 100‑block groups   */
    idx_t offset = m % 80;          /* index within the current block (0‑79) */

    seq_t res = (seq_t)full * (seq_t)100 + (seq_t)20;      /* block start + 20 */
    res += (seq_t)10 * (seq_t)(offset / 10);              /* tens digit offset  */
    res += (seq_t)(offset % 10);                          /* units digit offset */
    return res;
}
