/*
 * A106916  Primes of the form 3x^2-2xy+5y^2, with x and y nonnegative.
 * https://oeis.org/A106916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) are the primes p representable as 3x² − 2xy + 5y² with
 *            non‑negative integers x and y. This function returns the n‑th
 *            such prime for any n ≥ 1
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime, isqrt_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/isqrt_uint64.h"
#include "../utils/is_prime.h"

static bool representable_uint64(uint64_t p)
{
    /* For each y we solve 3x² − 2yx + (5y² − p) = 0.
       The discriminant is D = 12p − 56y².
       A solution exists iff D is a perfect square and
       either (2y + sqrt(D)) or (2y − sqrt(D)) is divisible by 6
       (and for the minus sign sqrt(D) ≤ 2y to keep x ≥ 0). */
    for (uint64_t y = 0; ; ++y)
    {
        __int128 temp = (__int128)12 * p - (__int128)56 * y * y;
        if (temp < 0) break;
        uint64_t D = (uint64_t)temp;
        uint64_t s = isqrt_uint64(D);
        if (s * s == D)
        {
            if ((2 * y + s) % 6 == 0) return true;
            if (2 * y >= s && (2 * y - s) % 6 == 0) return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static state preserved between calls. */
    static seq_t *prime_list = NULL;
    static size_t prime_cap   = 0;
    static size_t prime_cnt   = 0;
    static uint64_t candidate = 2;      /* last tested number */

    /* Generate primes until we have at least n of them. */
    while (prime_cnt < n)
    {
        ++candidate;
        if (!is_prime(candidate)) continue;   /* skip composites */

        if (representable_uint64(candidate))
        {
            if (prime_cnt == prime_cap)
            {
                prime_cap = prime_cap ? prime_cap * 2 : 256;
                seq_t *tmp = (seq_t *)realloc(prime_list,
                                              prime_cap * sizeof(seq_t));
                if (!tmp) return 0;            /* allocation failure */
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = (seq_t)candidate;
        }
    }
    return prime_list[n - 1];
}
