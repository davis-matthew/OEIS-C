/*
 * A021122  Decimal expansion of 1/118.
 * https://oeis.org/A021122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * (10^n mod 118) / 118)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute r = 10^n mod 118 using binary exponentiation */
    unsigned long long r = 1;
    unsigned long long base = 10;
    idx_t e = n;
    while (e) {
        if (e & 1ULL)
            r = (r * base) % 118ULL;
        base = (base * base) % 118ULL;
        e >>= 1;
    }
    /* Digit extraction */
    return (seq_t)((10ULL * r) / 118ULL);
}
