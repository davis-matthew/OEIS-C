/*
 * A076824  Let a(1)=a(2)=1, a(n)=(2^ceiling(a(n-1)/2)+1)/a(n-2).
 * https://oeis.org/A076824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=a(2)=1, a(n) = (2^ceil(a(n-1)/2)+1)/a(n-2); The sequence is
 *            5‑periodic with period (1, 1, 3, 5, 3)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Map n→(n‑1) mod 5 to get the appropriate term */
    static const seq_t period[5] = { (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)5, (seq_t)3 };
    return period[(n - 1) % 5];
}
