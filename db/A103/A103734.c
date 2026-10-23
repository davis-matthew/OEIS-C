/*
 * A103734  Negative of column k=7 sequence of array A103728.
 * https://oeis.org/A103734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -(18108 - 26200*p + 19901*p^2 - 8729*p^3 + 2282*p^4 * -
 *            350*p^5 + 29*p^6 - p^7) / 7! , where p is the prime at index n+5
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 18, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Prime storage (1‑based indices). */
static uint64_t *prime_list   = NULL;
static idx_t     prime_count   = 0;
static idx_t     prime_cap     = 0;

/* Ensure that at least `needed` primes are generated. */
static void
ensure_primes(idx_t needed)
{
    if (prime_count >= needed)
        return;

    uint64_t candidate;

    if (prime_count == 0) {          /* first time: add 2 */
        if (prime_cap == 0) {
            prime_cap = 16;
            prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
            if (!prime_list) abort();
        }
        prime_list[prime_count++] = 2ULL;
        candidate = 3ULL;
    } else {
        candidate = prime_list[prime_count - 1] + 1;
        if (candidate == 2) candidate = 3;
    }

    while (prime_count < needed) {
        /* Skip even numbers. */
        if ((candidate & 1ULL) == 0)
            candidate++;

        while (!is_prime(candidate))          /* helper from utils/is_prime.h */
            candidate += 2;

        if (prime_count == prime_cap) {
            prime_cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            if (!tmp) abort();
            prime_list = tmp;
        }

        prime_list[prime_count++] = candidate;
        candidate += 2;
    }
}

/* Return the (idx)-th prime (1‑based). */
static uint64_t
prime_at(idx_t idx)
{
    ensure_primes(idx);
    return prime_list[idx - 1];
}

/* Sequence term a(n). */
seq_t
a(idx_t n)
{
    const idx_t p_index = n + 5;      /* use prime n+5 (1‑based) */
    uint64_t p = prime_at(p_index);

    __int128 num = 18108;
    num -= (__int128)26200 * p;
    num += (__int128)19901 * ( (__int128)p * p );
    num -= (__int128)8729  * ( (__int128)p * p * p );
    num += (__int128)2282  * ( (__int128)p * p * p * p );
    num -= (__int128)350   * ( (__int128)p * p * p * p * p );
    num += (__int128)29    * ( (__int128)p * p * p * p * p * p );
    num -= (__int128)1     * ( (__int128)p * p * p * p * p * p * p );

    /* 7! = 5040. The numerator is negative; the final result
     * is the positive quotient. */
    seq_t result = (seq_t)(-num / 5040);
    return result;
}
