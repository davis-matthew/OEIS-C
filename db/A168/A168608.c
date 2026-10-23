/*
 * A168608  Decimal expansion of average of two-digit primes.
 * https://oeis.org/A168608
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  cons, nonn, base, less
 * Formula:   a(n) = decimal digit n of the expansion of 1043/21 = 49.66666...
 *            with offset 2: a(2)=4, a(3)=9, a(n)=6 for all n≥4
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* undefined for indices < 2 */
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)9;
    return (seq_t)6;
}
