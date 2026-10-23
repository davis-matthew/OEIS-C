/*
 * A107196  Primes of the form 4x^2 + 21y^2.
 * https://oeis.org/A107196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 397, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime_uint64, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 4x^2 + 21y^2 (OEIS A107196).
   Computation: iterate odd integers, test primality, then test whether the
   prime can be written as 4x² + 21y² by checking all possible y up to
   ⌊√(p/21)⌋.  The found primes are stored in a lazily‑filled dynamic
   array, so calls in any order are supported. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrime_uint64.h"   /* isPrime_uint64(uint64_t) -> bool   */
#include "../utils/push_seq.h"         /* push_seq(seq_t **, idx_t *, idx_t *, seq_t) */

static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t low = 0, high = (n >> 1) + 1;
    while (low < high) {
        uint64_t mid = low + ((high - low + 1) >> 1);
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static bool is_square_uint64(uint64_t n)
{
    uint64_t r = isqrt_uint64(n);
    return r * r == n;
}

static bool good_representation(uint64_t p)
{
    /* y ranges 0 … floor(sqrt(p/21)) */
    for (uint64_t y = 0;; ++y) {
        __int128 ty = (__int128)21 * y * y;
        if (ty > p)
            break;
        uint64_t rem = p - (uint64_t)ty;
        /* remainder must be divisible by 4 */
        if (rem & 3ULL)
            continue;
        uint64_t x2 = rem >> 2;      /* x² = rem / 4 */
        if (is_square_uint64(x2))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* undefined for n=0, return 0 */
        return (seq_t)0;

    /* Persistent dynamic array of found primes */
    static seq_t *terms   = NULL;
    static idx_t terms_cap = 0, terms_size = 0;
    static idx_t last_checked = 3;   /* start with the first odd > 2 */

    /* Generate primes until we have at least n terms */
    while (terms_size < n) {
        uint64_t p = last_checked;
        last_checked += 2;              /* only odd candidates */

        if (!isPrime_uint64(p))
            continue;                   /* not a prime */

        if (!good_representation(p))
            continue;                   /* not representable by 4x²+21y² */

        /* Append the prime to the dynamic array */
        push_seq(&terms, &terms_cap, &terms_size, (seq_t)p);
    }

    return terms[n - 1];
}
