/*
 * A130719  a(n) = n-th digit after the decimal point of the decimal
 *          representation of the n-th harmonic number.
 * https://oeis.org/A130719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n-th digit after the decimal point of the n-th harmonic
 *            number H_n. It is computed as the n-th digit of the fractional
 *            part of H_n. The digit is obtained by evaluating floor(H_n *
 *            10^n) modulo 10. The computation uses modular arithmetic to
 *            avoid dealing with gigantic rational numbers. For...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static uint64_t mod_pow_uint64(uint64_t base, idx_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0)
    {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used, offset starts at 1 */

    uint64_t sum_floor_mod10 = 0;  /* sum of the last digits of floor(10^n/k) */
    long double frac_sum = 0.0L;   /* sum of the fractional remainders */

    for (idx_t k = 1; k <= n; ++k)
    {
        uint64_t r = mod_pow_uint64(10ULL, n, (uint64_t)k);
        uint64_t m = mod_pow_uint64(10ULL, n, 10ULL * k);
        uint64_t q = (m - r) / k;          /* 0 <= q <= 9 in fact */

        sum_floor_mod10 += q;
        if (sum_floor_mod10 >= 10)        /* keep modulo 10 to avoid overflow */
            sum_floor_mod10 -= 10 * (sum_floor_mod10 / 10);

        frac_sum += (long double)r / (long double)k;
    }

    uint64_t carry = (uint64_t)floorl(frac_sum + 1e-12L);   /* integer part of the fractional sum */
    uint64_t result = (sum_floor_mod10 + carry) % 10;

    return (seq_t)result;
}
