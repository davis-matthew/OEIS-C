/*
 * A122008  Expansion of (2*x-1)*(x-1)*x / ((3*x-1)*(3*x^2-1)).
 * https://oeis.org/A122008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)+3*a(n-2)-9*a(n-3) for n>3 initial values:
 *            a(1)=1, a(2)=0, a(3)=5
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)5;

    /* iterate the cubic recurrence for n ≥ 4 */
    seq_t a_nm3 = (seq_t)1;  /* a(n-3) */
    seq_t a_nm2 = (seq_t)0;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)5;  /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)3 * a_nm1 + (seq_t)3 * a_nm2 - (seq_t)9 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
