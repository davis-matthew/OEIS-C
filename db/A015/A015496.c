/*
 * A015496  a(0)=1, a(1)=8, a(n) = sum_{k=0}^{k=n-1} 8^k a(k).
 * https://oeis.org/A015496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=8, a(2)=65;
   for n>=3, a(n) = a(n-1)*(1+8^(n-1)) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)65;  /* 1 + 8*8 */

    seq_t result  = (seq_t)65;    /* a(2) */
    seq_t power8  = (seq_t)64;    /* 8^2 */

    for (idx_t i = 3; i <= n; ++i) {
        result = result * (power8 + (seq_t)1);
        power8  = power8 * (seq_t)8;
    }
    return result;
}
