/*
 * A193910  Leap centuries in the revised Julian calendar.
 * https://oeis.org/A193910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For the revised Julian leap-century sequence (A193910) a(1) = 2,
 *            a(2) = 6, and for n ≥ 3 a(n) = a(n-2) + 9. Closed form: a(2*k-1)
 *            = 9*k - 7 (odd indices) a(2*k) = 9*k - 3 (even indices)
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety, return 0 if the caller passes 0 (outside the defined range). */
    if (n == 0) return (seq_t)0;

    if (n & 1) {                 /* n is odd : n = 2*k-1  */
        seq_t k = ((seq_t)n + (seq_t)1) / (seq_t)2;  /* k = (n+1)/2 */
        return (seq_t)9 * k - (seq_t)7;
    } else {                    /* n is even : n = 2*k  */
        seq_t k = (seq_t)n / (seq_t)2;                /* k = n/2 */
        return (seq_t)9 * k - (seq_t)3;
    }
}
