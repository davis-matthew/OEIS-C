/*
 * A092898  Expansion of (1 - 4*x + 4*x^2 - 4*x^3)/(1 - 4*x).
 * https://oeis.org/A092898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=4, a(3)=12
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4 * a(n-1)   for n >= 4 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)12;

    seq_t val = (seq_t)12;
    for (idx_t i = 4; i <= n; ++i)
        val *= (seq_t)4;
    return val;
}
