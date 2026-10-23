/*
 * A033422  a(n) = floor(10000/n).
 * https://oeis.org/A033422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(10000 / n) – OEIS sequence A033422
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1 ⟹ floor(10000 / n). */
    seq_t res = (seq_t)(10000ULL / (uint64_t)n);
    return res;
}
