/*
 * A097739  Pell equation solutions (9*b(n))^2 - 82*a(n)^2 = -1 with b(n) =
 *          A097738(n), n >= 0.
 * https://oeis.org/A097739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=325; for n>1, a(n)=326*a(n-1)-a(n-2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)325;

    const seq_t mult = (seq_t)326;
    seq_t prev = (seq_t)1;    /* a(0) */
    seq_t curr = (seq_t)325;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = mult * curr - prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
