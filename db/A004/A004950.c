/*
 * A004950  a(n) = round(n*phi^15), where phi is the golden ratio, A001622.
 * https://oeis.org/A004950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n * phi^15), where phi^15 = 682 + 305*sqrt(5)
 * Range:     u8 n <= 0, u16 n <= 48, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    return (n_seq * 13640007064892ull + 5000000000ull) / 10000000000ull;
}
