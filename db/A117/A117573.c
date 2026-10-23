/*
 * A117573  Expansion of (1+2x^2)/((1-x)(1-x^2)(1-x^3)).
 * https://oeis.org/A117573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6); a(0)=1, a(1)=1,
 *            a(2)=4, a(3)=5, a(4)=8, a(5)=11
 * Range:     u8 n <= 30, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Directly return the first six values. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)8;
    if (n == 5) return (seq_t)11;

    /* Use a circular buffer of the last six terms to implement the recurrence. */
    seq_t buf[6] = { (seq_t)1, (seq_t)1, (seq_t)4, (seq_t)5,
                     (seq_t)8, (seq_t)11 };

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = buf[(i-1)%6] + buf[(i-2)%6];
        next -= buf[(i-4)%6];
        next -= buf[(i-5)%6];
        next += buf[(i-6)%6];
        buf[i%6] = next;
    }
    return buf[n%6];
}
