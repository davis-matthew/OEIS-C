/*
 * A021215  Decimal expansion of 1/211.
 * https://oeis.org/A021215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * (10ⁿ mod 211) / 211) – decimal digit of 1/211
 *            after the decimal point
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute r = 10^n mod 211 using binary exponentiation. */
    uint64_t result = 1u;
    uint64_t base   = 10u % 211u;
    idx_t       exp  = n;

    while (exp) {
        if (exp & 1u) {
            result = (result * base) % 211u;
        }
        base = (base * base) % 211u;
        exp >>= 1;
    }

    /* The nth digit is floor((r * 10) / 211). */
    uint64_t digit = (result * 10u) / 211u;
    return (seq_t)digit;
}
