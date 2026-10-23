/*
 * A047240  Numbers that are congruent to {0, 1, 2} mod 6.
 * https://oeis.org/A047240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*floor((n-1)/3)+((n-1)%3)
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n must be >= 1 */
    idx_t k   = n - 1;                /* shift to 0‑based index */
    seq_t kseq = (seq_t)k;            /* promote for seq_t arithmetic */
    seq_t q = kseq / 3;               /* floor((n-1)/3) */
    seq_t r = kseq % 3;               /* (n-1) mod 3 */
    return q * 6 + r;
}
