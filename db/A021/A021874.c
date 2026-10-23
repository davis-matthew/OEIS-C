/*
 * A021874  Expansion of 1/((1-x) * (1-4*x) * (1-7*x) * (1-10*x)).
 * https://oeis.org/A021874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^(n+3) - 3*7^(n+3) + 3*4^(n+3) - 1) / 162
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(n) = 22*a(n-1) - 159*a(n-2) + 418*a(n-3) - 280*a(n-4) */
/* Base terms: a(0)=1, a(1)=22, a(2)=325, a(3)=4070 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;
    if (n == 2) return (seq_t)325;
    if (n == 3) return (seq_t)4070;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)22;
    seq_t a2 = (seq_t)325;
    seq_t a3 = (seq_t)4070;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a3 * 22 - a2 * 159 + a1 * 418 - a0 * 280;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
