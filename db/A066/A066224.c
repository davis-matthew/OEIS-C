/*
 * A066224  Bisection of A000085.
 * https://oeis.org/A066224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, * a(n+2) = 4*(n+2)*a(n+1) - 2*(n+1)*(2*n+3)*a(n)
 *            (for n ≥ 0) * This recurrence generates A066224. It runs in O(n)
 *            time and uses only * integer additions and multiplications;
 *            intermediate results stay * within the range of the target type
 *            for all terms that fit in seq_t
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t a0 = (seq_t)1;          /* a(0) */
    seq_t a1 = (seq_t)4;          /* a(1) */

    for (idx_t i = 0; i + 2 <= n; ++i) {
        seq_t part1 = (seq_t)4 * (seq_t)(i + 2) * a1;
        seq_t part2 = (seq_t)2 * (seq_t)(i + 1) * (seq_t)(2 * i + 3) * a0;
        seq_t next = part1 - part2;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
