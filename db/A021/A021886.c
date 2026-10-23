/*
 * A021886  Decimal expansion of 1/882.
 * https://oeis.org/A021886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/882) % 10 (decimal digit of 1/882 after
 *            the decimal point)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow10_mod(idx_t exp, seq_t mod)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)10 % mod;
    while (exp) {
        if (exp & 1) {
            idx_t prod = ((idx_t)result * (idx_t)base) % (idx_t)mod;
            result = (seq_t)prod;
        }
        exp >>= 1;
        if (exp) {
            idx_t prod = ((idx_t)base * (idx_t)base) % (idx_t)mod;
            base = (seq_t)prod;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t r = pow10_mod(n, (seq_t)882);         /* 10^n mod 882    */
    idx_t digit = (((idx_t)r) * 10ULL) / 882ULL; /* floor(10*r/882) */
    return (seq_t)digit;
}
