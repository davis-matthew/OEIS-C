/*
 * A057960  Number of base-5 (n+1)-digit numbers starting with a zero and with
 *          adjacent digits differing by one or less.
 * https://oeis.org/A057960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-3) (n ≥ 3), a(0)=1, a(1)=2, a(2)=5
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)2;   /* a(n-2) */
    seq_t a2 = (seq_t)5;   /* a(n-1) */
    seq_t an;              /* a(n)  */

    for (idx_t i = 3; i <= n; ++i) {
        an = a2 * (seq_t)3 - a0 * (seq_t)2;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
