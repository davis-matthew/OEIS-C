/*
 * A074500  Difference between n*sqrt(n)+1 and prime(n), rounded to nearest
 *          integer.
 * https://oeis.org/A074500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*sqrt(n)+1 - prime(n))
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

/* Integer square root of a 128-bit integer: floor(sqrt(x)). */
static uint64_t isqrt128(__int128 x)
{
    if (x == 0) return 0;
    uint64_t low = 0, high = ((uint64_t)1 << 63);
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __int128 mid2 = (__int128)mid * mid;
        if (mid2 <= x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* Static storage for primes produced by build_primes. */
static idx_t *primes_arr   = NULL;
static size_t primes_cnt   = 0;
static idx_t  primes_limit = 0;
static bool   primes_ready = false;

/* Ensure that the array primes_arr contains at least `need` primes. */
static void ensure_primes(idx_t need)
{
    if (!primes_ready) {
        /* Choose an initial limit that is sufficient for normal use. */
        primes_limit = 20000;                     /* < sufficient for n≈10000 */
        build_primes(primes_limit, &primes_arr, &primes_cnt);
        primes_ready = true;
    }
    /* Grow the limit until we have at least `need` primes. */
    while (need > primes_cnt) {
        primes_limit <<= 1;                     /* double the limit */
        free(primes_arr);
        build_primes(primes_limit, &primes_arr, &primes_cnt);
    }
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* offset is 1 */

    /* Compute N = n^3 in 128-bit so it does not overflow. */
    __int128 N = (__int128)n * n * n;

    /* s = floor(sqrt(N)). */
    uint64_t s = isqrt128(N);

    /* Determine whether the fractional part of sqrt(N) is >= 0.5. */
    __int128 thresh = (__int128)s * s + (__int128)s + 1; /* s^2 + s + 1 */
    uint64_t flag  = (N >= thresh) ? 1 : 0;

    /* T = floor(s + 1.5) = s + 1 + flag. */
    __int128 T_big = (__int128)s + 1 + flag;
    seq_t T = (seq_t)T_big;

    /* Get prime(n) (1‑based index). */
    ensure_primes(n);
    seq_t P = (seq_t)primes_arr[n - 1];

    /* Compute the difference.  `T` and `P` are non‑negative and fit
       into the signed 128‑bit type, so subtraction is safe. */
    __int128 diff = (__int128)T - (__int128)P;
    return (seq_t)diff;
}
