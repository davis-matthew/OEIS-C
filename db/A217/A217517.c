/*
 * A217517  Base-n state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (12345)*.
 * https://oeis.org/A217517
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = [20,20,10,6,5] repeated every 5 starting at n=2 (i.e.,
 *            a(n+5)=a(n) for n≥2)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* undefined for n < 2; behavior does not matter */
    static const seq_t values[5] = {20, 20, 10, 6, 5};
    return values[(n - 2) % 5];
}
