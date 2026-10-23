/*
 * A135918  Genus of stage-n Menger sponge.
 * https://oeis.org/A135918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (21*20^n + 38*8^n - 59)/133
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 3‑term recurrence: a(0)=0, a(1)=5, a(2)=81,
   a(n)=28*a(n-1)-160*a(n-2)-59 for n≥3 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)81;

    seq_t prev2 = (seq_t)5;   /* a(1) */
    seq_t prev1 = (seq_t)81;  /* a(2) */
    seq_t cur = prev1;        /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)28 * prev1 - (seq_t)160 * prev2 - (seq_t)59;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
