/*
 * A106946  Primes of the form 2x^2+xy+9y^2, with x and y any integer.
 * https://oeis.org/A106946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 937, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_seq_array, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106946: primes of the form 2x^2 + xy + 9y^2, found by a linear
   search of primes and testing representability.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include "../utils/is_prime.h"
#include "../utils/ensure_seq_array.h"

static seq_t *seq = NULL;          /* array of computed terms            */
static size_t cap = 0;             /* current capacity of the array      */
static size_t size = 0;            /* number of terms actually stored    */
static uint64_t next_candidate = 2;/* next integer to test for primality */

/* integer square root for 64‑bit unsigned integers */
static inline uint64_t isqrt64(uint64_t x)
{
    uint64_t r = 0, bit = (uint64_t)1 << 62;
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* integer square root for unsigned __int128 values */
static inline uint64_t isqrt128(unsigned __int128 v)
{
    uint64_t low = 0, high = (uint64_t)1 << 34;   /* > sqrt(2^67) */
    while (low < high) {
        uint64_t mid = (low + high) >> 1;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= v) low = mid + 1;
        else           high = mid;
    }
    return low - 1;
}

/* Does the prime p admit an integer solution of 2x^2 + xy + 9y^2 = p ? */
static bool representable(uint64_t p)
{
    unsigned __int128 tmp = (unsigned __int128)8 * p;
    uint64_t max_y = isqrt64((uint64_t)(tmp / 71));
    for (int64_t y = -(int64_t)max_y; y <= (int64_t)max_y; ++y) {
        unsigned __int128 ysq = (unsigned __int128)y * y;
        unsigned __int128 val = tmp - (unsigned __int128)71 * ysq;
        uint64_t t = isqrt128(val);
        if ((unsigned __int128)t * t != val) continue;
        int64_t d1 = (int64_t)t - y;
        if ((d1 & 3LL) == 0) return true;
        int64_t d2 = (int64_t)t + y;
        if ((d2 & 3LL) == 0) return true;
    }
    return false;
}

/* Ensure that the array has capacity for at least 'need' elements.  */
static void ensure_capacity(size_t need)
{
    if (cap >= need) return;
    size_t new_cap = cap ? cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    ensure_seq_array(&seq, &cap, new_cap);
}

/* -------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (size < (size_t)n) {
        if (is_prime(next_candidate) && representable(next_candidate)) {
            ensure_capacity(size + 1);
            seq[size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq[n - 1];
}
