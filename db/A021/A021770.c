/*
 * A021770  Decimal expansion of 1/766.
 * https://oeis.org/A021770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^{n+1} / 766) % 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the (n+1)-st decimal digit of 1/766. */
seq_t a(idx_t n)
{
    /* Constants */
    const unsigned long long MOD = 766ULL;

    /* Compute r = 10^n mod MOD using binary exponentiation */
    unsigned long long r = 1ULL;
    unsigned long long base = 10ULL % MOD;
    idx_t exp = n;

    while (exp) {
        if (exp & 1ULL)
            r = (r * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }

    /* Digit is floor((r * 10) / MOD) */
    unsigned long long digit = (r * 10ULL) / MOD;

    /* Result is a single decimal digit */
    return (seq_t)digit;
}
