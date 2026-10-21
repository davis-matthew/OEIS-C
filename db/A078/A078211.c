/*
 * A078211  Smallest multiple of n^2 beginning and ending in n, or 0 if no
 *          such multiple exists.
 * https://oeis.org/A078211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest multiple of n^2 that starts and ends in the
 *            decimal representation of n, or 0 if it does not exist
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   gcd_uint64, modinv_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"
#include "../utils/modinv_uint64.h"

/* local helpers */
static uint64_t digits_uint64(uint64_t x)
{
    uint64_t d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1, but be safe */
    uint64_t n64 = (uint64_t)n;
    uint64_t d = digits_uint64(n64);          /* digits of n */
    uint64_t pow10_d = 1;
    for (uint64_t i = 0; i < d; ++i) pow10_d *= 10;   /* 10^d */

    uint64_t n2 = n64 * n64;               /* n^2 fits in 64 bits for n <= 10000 */

    uint64_t g = gcd_uint64(n2, pow10_d);
    if (n64 % g != 0) return 0;           /* no solution */

    /* reduced congruence parameters */
    uint64_t a1 = n2 / g;
    uint64_t b1 = pow10_d / g;
    uint64_t c1 = n64 / g;

    uint64_t inv = modinv_uint64(a1 % b1, b1);
    uint64_t t0 = (c1 * inv) % b1;       /* minimal solution modulo b1 */

    /* now search over increasing number of digits k of the result */
    unsigned __int128 pow10_kd = 1;      /* 10^(k-d), starts with k=d => 10^0 = 1 */
    for (uint64_t k = d; ; ++k) {
        /* compute lower and upper bounds on t based on leading digit constraint */
        unsigned __int128 num_low  = (unsigned __int128)n64 * pow10_kd;
        unsigned __int128 num_high = (unsigned __int128)(n64 + 1) * pow10_kd - 1;

        unsigned __int128 t_low  = (num_low + n2 - 1) / n2;   /* ceil */
        unsigned __int128 t_high = num_high / n2;            /* floor */

        if (t_low <= t_high) {
            /* find smallest t in [t_low,t_high] that ≡ t0 (mod b1) */
            unsigned __int128 t_candidate;
            if (t_low <= t0) {
                t_candidate = t0;
            } else {
                unsigned __int128 delta = t_low - t0;
                unsigned __int128 step  = (delta + b1 - 1) / b1;
                t_candidate = t0 + step * b1;
            }
            if (t_candidate <= t_high) {
                /* found minimal N = n^2 * t_candidate */
                seq_t result = (seq_t)n2 * (seq_t)t_candidate;
                return result;
            }
        }

        /* prepare for next k: increase digit count by 1 */
        pow10_kd *= 10;
        /* sanity stop: if pow10_kd becomes too large for unsigned __int128
           which would cause overflow in further operations, we stop. */
        if (pow10_kd > ((unsigned __int128)1 << 120)) break; /* arbitrarily large */
        /* also break if we have expanded many digits with no success */
        if (k > d + 25) break; /* heuristic limit ensuring termination */
    }
    return 0;   /* no valid multiple found */
}
