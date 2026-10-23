/*
 * A010921  Shallit sequence S(3,13), a(n)=[ a(n-1)^2/a(n-2)+1 ].
 * https://oeis.org/A010921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=3, a(1)=13, a(n)=floor(a(n-1)^2/a(n-2))+1 for n>=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __cplusplus
/* C implementation – use 128‑bit intermediate to avoid overflow */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)13;

    seq_t prev2 = (seq_t)3;
    seq_t prev1 = (seq_t)13;
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        unsigned __int128 prod = (unsigned __int128)prev1 * prev1;
        cur = (seq_t)(prod / prev2 + 1);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
#else
/* C++ implementation – seq_t is an arbitrary‑precision integer class */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)13;

    seq_t prev2 = (seq_t)3;
    seq_t prev1 = (seq_t)13;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (prev1 * prev1) / prev2 + 1;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
#endif
