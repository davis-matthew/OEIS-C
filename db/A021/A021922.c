/*
 * A021922  Decimal expansion of 1/918.
 * https://oeis.org/A021922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/918) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute 10^exp mod 9180 using binary exponentiation.
   All intermediate values fit in uint64_t. */
static unsigned long long pow10mod(idx_t exp)
{
    const unsigned long long MOD = 9180ULL;   /* 918 * 10 */
    unsigned long long result = 1ULL;
    unsigned long long base = 10ULL;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 10^n mod 9180 */
    unsigned long long r = pow10mod(n);
    /* 10^(n+1) mod 9180 */
    r = (r * 10ULL) % 9180ULL;
    /* digit = (10^(n+1) / 918) mod 10 */
    return (seq_t)(r / 918ULL);   /* 0 .. 9 fits in any seq_t */
}
