/*
 * A105186  Replace odd-positioned digits with 0 in ternary representation of
 *          n.
 * https://oeis.org/A105186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9 * a(floor(n/9)) + (n mod 9) mod 3 a(0) = 0 Recurrence
 *            works for all n ≥ 0
 * Range:     u8 n <= 728, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 9)
        return (seq_t)(n % 3);
    seq_t sub = a(n / 9);
    seq_t r   = (seq_t)((n % 9) % 3);
    return sub * ((seq_t)9) + r;
}
