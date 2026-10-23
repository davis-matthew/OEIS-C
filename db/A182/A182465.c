/*
 * A182465  a(n) = 3*a(n-1) - 2*a(n-2) with a(0)=28 and a(1)=70.
 * https://oeis.org/A182465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 14, a(0)=28, a(1)=70
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)28;
    if (n == (idx_t)1) return (seq_t)70;

    seq_t curr = (seq_t)70;        /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        curr = curr * (seq_t)2 + (seq_t)14;   /* a(i) = 2*a(i‑1)+14 */
    }
    return curr;
}
