/*
 * A021513  Decimal expansion of 1/509.
 * https://oeis.org/A021513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * (10^n mod 509) / 509)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This gives the (n+1)-th digit after the decimal point in the expansion of 1/509. */

static inline seq_t pow10_mod(idx_t e)
{
    seq_t result = 1;
    seq_t base   = 10;
    while (e > 0)
    {
        if (e & 1)          /* if the lowest bit of e is set */
            result = (result * base) % 509;
        base = (base * base) % 509;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t rem   = pow10_mod(n);          /* 10^n mod 509 (0 .. 508)  */
    seq_t digit = (rem * (seq_t)10) / (seq_t)509;  /* 0 .. 9 */
    return digit;
}
