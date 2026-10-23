/*
 * A160542  Not divisible by 11.
 * https://oeis.org/A160542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n-1)/10) (numbers not divisible by 11)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour below offset is undefined */
    seq_t sn = (seq_t)n;            /* cast index to value type */
    seq_t inc = (sn - 1) / 10;      /* number of skipped multiples of 11 */
    return sn + inc;                /* nth non‑multiple of 11 */
}
