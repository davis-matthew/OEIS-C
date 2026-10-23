/*
 * A015710  Least k >= 0 such that phi(n) * sigma(n) + k^2 is a perfect
 *          square, or -1 if impossible.
 * https://oeis.org/A015710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least k ≥ 0 such that φ(n)*σ(n) + k² is a perfect square,
 *            or -1 if no such k exists
 * Range:     i8 n <= 82, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------- */
/*  Portable integer square‑root for 64‑bit values                     */
/* --------------------------------------------------------------- */
#include <stdint.h>   /* for uint64_t, etc. */

static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t low = 0, high = x;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq  = (__uint128_t)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;    /* floor(sqrt(x)) */
}

/* --------------------------------------------------------------- */
/*  Euler's totient φ(n) – computed by trial division                */
/* --------------------------------------------------------------- */
static inline uint64_t phi_uint64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = n;
    uint64_t temp   = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1) result -= result / temp;
    return result;
}

/* --------------------------------------------------------------- */
/*  Sum of divisors σ(n) – computed by trial division                */
/* --------------------------------------------------------------- */
static inline uint64_t sigma_uint64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = 1;
    uint64_t temp   = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t term   = 1;
            uint64_t power  = 1;
            while (temp % p == 0) {
                temp  /= p;
                power *= p;
                term  += power;
            }
            result *= term;
        }
    }
    if (temp > 1) {
        result *= (1 + temp);
    }
    return result;
}

/* --------------------------------------------------------------- */
/*  Main OEIS sequence function                                    */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return -1;          /* undefined index 0 */

    /* Compute φ(n) and σ(n) using 64‑bit arithmetic to avoid overflow */
    uint64_t phi   = phi_uint64((uint64_t)n);
    uint64_t sigma = sigma_uint64((uint64_t)n);
    __uint128_t t128 = (__uint128_t)phi * sigma;
    if (t128 == 0) return -1;

    /* Work with 64‑bit t since φ(n)*σ(n) ≤ 10⁹ for n ≤ 10000 */
    uint64_t t = (uint64_t)t128;
    uint64_t limit = isqrt_u64(t);

    uint64_t best_k = (uint64_t)-1;   /* sentinel: no solution yet */

    for (uint64_t d = 1; d <= limit; ++d) {
        if (t % d != 0) continue;
        uint64_t q = t / d;
        uint64_t diff = q - d;           /* diff ≥ 0 */
        if ((diff & 1ULL) == 0ULL) {     /* diff even => integer k */
            uint64_t k = diff >> 1;
            if (best_k == (uint64_t)-1 || k < best_k) {
                best_k = k;
                if (best_k == 0ULL) break;   /* cannot get smaller than 0 */
            }
        }
    }

    /* Convert result to seq_t (may overflow on small seq_t builds,
       which is acceptable as per problem statement). */
    if (best_k == (uint64_t)-1) return (seq_t)-1;
    return (seq_t)best_k;
}
