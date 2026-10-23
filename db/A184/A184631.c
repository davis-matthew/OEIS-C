/*
 * A184631  Floor(1/{(7+n^4)^(1/4)}), where {}=fractional part.
 * https://oeis.org/A184631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(4*n^3/7) for n >= 3 Special values: a(1) = 1, a(2)
 *            = 5 Recurrence from OEIS:
 *            a(n)=3a(n-1)-3a(n-2)+a(n-3)+a(n-7)-3a(n-8)+3a(n-9)-a(n-10),
 *            n>=13
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1818, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for n<1, harmless */
    if (n == 1) return 1;
    if (n == 2) return 5;

    /* Compute floor(4 * n^3 / 7) using only integer arithmetic. */
    seq_t nn = (seq_t)n;
    seq_t cube = nn * nn * nn;   /* n^3 */
    seq_t val = cube * (seq_t)4; /* 4*n^3 */
    return val / (seq_t)7;
}
