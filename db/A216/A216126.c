/*
 * A216126  a(n) = 5^n mod 10000.
 * https://oeis.org/A216126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 5^n mod 10000
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 64‑bit arithmetic for intermediate results to avoid overflow */
    unsigned long long mod  = 10000ULL;
    unsigned long long base = 5ULL % mod;
    unsigned long long exp  = (unsigned long long)n;
    unsigned long long result = 1ULL;

    while (exp > 0ULL) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }

    return (seq_t)result;
}
