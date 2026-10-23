/*
 * A154304  Primes of the form (1+2+...+m)/210 = A000217(m)/210.
 * https://oeis.org/A154304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = prime of the form m(m+1)/420; the m(m+1)/420 value must
 *            be an integer prime, enumerated by increasing m
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit values. */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    uint64_t d = 3;
    while (d <= x / d) {
        if (x % d == 0) return false;
        d += 2;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1 – callers must use n >= 1 */

    /* Static cache to store primes already discovered. */
    static seq_t cache[64];
    static size_t cache_len = 0;          /* number of primes stored */
    static idx_t last_checked_m = 0;      /* last m that was examined */

    /* Search for more primes until we have at least n of them or we
       pass a safe upper bound.  Past m = 420, m(m+1)/420 cannot be a
       prime, so we stop after a generous safety margin. */
    while (cache_len < n && last_checked_m < 600) {
        ++last_checked_m;
        uint64_t mm   = last_checked_m;
        uint64_t prod = mm * (mm + 1);    /* m*(m+1) */
        if (prod % 420ULL == 0) {
            uint64_t p = prod / 420ULL;   /* candidate prime */
            if (is_prime_uint64(p)) {
                cache[cache_len++] = (seq_t)p;
            }
        }
    }

    /* If n is beyond the known sequence length, return 0. */
    if (n == 0 || n > cache_len) return 0;

    return cache[n - 1];
}
