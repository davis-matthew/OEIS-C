/*
 * A191402  a(n) = A000201(n) + A000201(n+1).
 * https://oeis.org/A191402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*phi) + floor((n+1)*phi) * where phi =
 *            (1+sqrt(5))/2. * The function computes this value using only
 *            integer arithmetic. * It pre‑computes a scaled integer
 *            approximation of phi, then * evaluates the two floor terms by
 *            integer division. The scaled * value is accurate enough to give
 *            th...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const idx_t SCALE = ((idx_t)1) << 40; /* 2^40, large enough for accurate scaling */

/* Compute integer square root of a non‑negative __int128 using binary search. */
static uint64_t isqrt128(__int128 n)
{
    __int128 low = 0, high = (__int128)SCALE * 3 + 1; /* sqrt(5)*SCALE < 3*SCALE */
    while (low < high) {
        __int128 mid = (low + high) >> 1;
        if (mid * mid <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return (uint64_t)(low - 1);
}

/* Compute scaled phi once; result fits into uint64_t. */
static uint64_t phi_scaled(void)
{
    static bool initialized = false;
    static uint64_t value = 0;
    if (!initialized) {
        /* phi_scaled = floor((SCALE + sqrt(5)*SCALE) / 2) */
        __int128 target = (__int128)5 * SCALE * SCALE; /* (sqrt(5)*SCALE)^2 */
        uint64_t sqrt5_scaled = isqrt128(target);       /* floor(sqrt(5)*SCALE)   */
        value = (SCALE + sqrt5_scaled) >> 1;            /* integer division by 2 */
        initialized = true;
    }
    return value;
}

/* Return the nth term of the sequence A191402. */
seq_t a(idx_t n)
{
    uint64_t perscale = phi_scaled();        /* scaled phi */
    __int128 prod_n    = (__int128)n * perscale;
    __int128 prod_n1   = (__int128)(n + (idx_t)1) * perscale;

    seq_t term1 = (seq_t)(prod_n / SCALE);
    seq_t term2 = (seq_t)(prod_n1 / SCALE);

    return term1 + term2;
}
