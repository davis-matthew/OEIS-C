/*
 * A011749  Expansion of 1/(1 + x^3 + x^5) mod 2.
 * https://oeis.org/A011749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1,a(1)=0,a(2)=0,a(3)=1,a(4)=0; for n>=5: a(n)=a(n-3)+a(n-5)
 *            mod 2
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)0;
    seq_t buf[5];
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)0;
    buf[2] = (seq_t)0;
    buf[3] = (seq_t)1;
    buf[4] = (seq_t)0;
    for (idx_t i = 5; i <= n; ++i) {
        seq_t val = ((buf[(i-3) % 5] + buf[(i-5) % 5]) & (seq_t)1);
        buf[i % 5] = val;
    }
    return buf[n % 5];
}
