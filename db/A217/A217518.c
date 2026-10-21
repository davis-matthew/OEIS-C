/*
 * A217518  Base-n state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (123456)*.
 * https://oeis.org/A217518
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = periodic sequence with period 6: a(2)=13, a(3)=7, a(4)=7,
 *            a(5)=12, a(6)=7, a(7)=6, then repeats. Equivalent to a(n) =
 *            tbl[(n-2) % 6] with tbl[6] = {13, 7, 7, 12, 7, 6}
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;               /* value irrelevant below offset */
    static const seq_t tbl[6] = {
        (seq_t)13, (seq_t)7, (seq_t)7,
        (seq_t)12, (seq_t)7, (seq_t)6
    };
    return tbl[(n - 2) % 6];
}
