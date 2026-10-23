/*
 * A021592  Decimal expansion of 1/588.
 * https://oeis.org/A021592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/588) mod 10
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* nth decimal digit of 1/588 */
seq_t a(idx_t n)
{
    seq_t rem = (seq_t)1;
    seq_t dig = 0;
    for (idx_t i = 0; i <= n; ++i) {
        rem *= (seq_t)10;
        dig  = rem / (seq_t)588;
        rem  = rem % (seq_t)588;
    }
    return dig;
}
