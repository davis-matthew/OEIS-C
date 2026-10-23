/*
 * A021566  Decimal expansion of 1/562.
 * https://oeis.org/A021566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021566: decimal expansion of 1/562.
   Compute nth digit using modular arithmetic:
   a(n) = floor(10 * (10^n mod 562) / 562)   */

static seq_t pow10mod(idx_t exp)
{
    /* Compute 10^exp mod 562 efficiently. */
    seq_t result = 1;
    seq_t base   = 10 % 562;      /* base = 10 */
    idx_t e = exp;
    while (e > 0) {
        if (e & 1)
            result = (result * base) % 562;
        base = (base * base) % 562;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t r = pow10mod(n);
    seq_t digit = (10 * r) / 562;
    return digit;
}
