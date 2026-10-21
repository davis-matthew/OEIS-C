/*
 * A056849  Final digit of n^n.
 * https://oeis.org/A056849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = last digit of n^n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed as n^n mod 10 using binary exponentiation with base reduced modulo 10. */
seq_t a(idx_t n)
{
    /* Reduce base to last digit */
    seq_t base = n % 10;
    /* Exponent is the original n */
    idx_t exp = n;

    /* Result starts at 1 mod 10 */
    seq_t result = 1 % 10;

    /* Binary exponentiation */
    while (exp > 0) {
        if (exp & 1u)
            result = (result * base) % 10;
        base = (base * base) % 10;
        exp >>= 1;
    }
    return result;
}
