/*
 * A097733  Pell equation solutions (7*b(n))^2 - 2*(5*a(n))^2 = -1 with b(n) =
 *          A097732(n), n >= 0. Note that D = 50 = 2*5^2 is not squarefree.
 * https://oeis.org/A097733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=197, a(n) = 198*a(n-1) - a(n-2) for n > 1
 *            (recurrence of order 2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first two terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)197;

    /* iterate the linear recurrence */
    seq_t prev = (seq_t)1;          /* a(0) */
    seq_t curr = (seq_t)197;       /* a(1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        next = (seq_t)198 * curr - prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
