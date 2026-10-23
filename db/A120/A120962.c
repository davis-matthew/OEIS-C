/*
 * A120962  Final digit (in decimal system) of n^(n^n), i.e., n^(n^n) mod 10.
 * https://oeis.org/A120962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^(n^n) mod 10
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute base^exp mod mod, with base, exp, mod up to 64‑bit */
static uint64_t modpow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* last digit of the base */
    uint8_t base = (uint8_t)(n % 10);

    /* trivial cases */
    if (base == 0 || base == 1 || base == 5 || base == 6)
        return (seq_t)base;

    /* determine the cycle length for the base */
    uint64_t period;
    switch (base)
    {
        case 4:
        case 9: period = 2; break;
        default: period = 4; /* 2,3,7,8 */
    }

    /* compute e = n^n mod period */
    uint64_t exp_mod = modpow_u64(n % period, n, period);

    /* choose the last digit according to the cycle */
    switch (base)
    {
        case 4:  /* cycle 4,6 */
            return (seq_t)(exp_mod == 0 ? 6 : 4);
        case 9:  /* cycle 9,1 */
            return (seq_t)(exp_mod == 0 ? 1 : 9);

        case 2:  /* cycle 2,4,8,6 */
            switch (exp_mod)
            {
                case 0: return (seq_t)6;
                case 1: return (seq_t)2;
                case 2: return (seq_t)4;
                case 3: return (seq_t)8;
            }
            break;

        case 3:  /* cycle 3,9,7,1 */
            switch (exp_mod)
            {
                case 0: return (seq_t)1;
                case 1: return (seq_t)3;
                case 2: return (seq_t)9;
                case 3: return (seq_t)7;
            }
            break;

        case 7:  /* cycle 7,9,3,1 */
            switch (exp_mod)
            {
                case 0: return (seq_t)1;
                case 1: return (seq_t)7;
                case 2: return (seq_t)9;
                case 3: return (seq_t)3;
            }
            break;

        case 8:  /* cycle 8,4,2,6 */
            switch (exp_mod)
            {
                case 0: return (seq_t)6;
                case 1: return (seq_t)8;
                case 2: return (seq_t)4;
                case 3: return (seq_t)2;
            }
            break;
    }

    /* fallback – should never happen */
    return (seq_t)0;
}
