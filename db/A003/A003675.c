/*
 * A003675  Decimal expansion of neutron mass (mass units).
 * https://oeis.org/A003675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = n-th digit of decimal expansion of neutron mass
 *            (1.00866491588...)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const char digits[] = 
        "1008664915888492167861529463914087267153418883221681881522084351490";
    
    if (n < 1 || n > 65)
        return 0;
    
    return (seq_t)(digits[n - 1] - '0');
}
