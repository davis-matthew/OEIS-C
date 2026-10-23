/*
 * A173737  a(n) = (10^n + 44)/9 for n>0.
 * https://oeis.org/A173737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n + 44)/9 (recurrence: a(0)=5, a(n)=10*a(n-1)-44 for
 *            n>0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)5;
    seq_t val = (seq_t)5;   /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        val = val * (seq_t)10 - (seq_t)44;
    }
    return val;
}
