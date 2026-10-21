/*
 * A237839  a(n) = |{0 < k <= n: q = |{p <= k*n: p and p + 2 are both prime}|
 *          and q + 2 are both prime}|.
 * https://oeis.org/A237839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ 0 < k ≤ n : let q = #{ prime p ≤ k·n | p and p+2 are
 *            prime } . Then q and q+2 are both prime }|
 * Range:     u8 n <= 160, u16 n <= 1100, u32 n <= 1100, u64 n <= 1100,
 *            u128 n <= 1100, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime_small.h"   /* is_prime_small(seq_t) */

static idx_t sieve_limit = 0;           /* current sieve upper bound          */
static uint8_t *sieve_bits = NULL;      /* bitset for odd composites (≥3)     */
static size_t sieve_bits_len = 0;       /* length in bytes                    */
static uint32_t *twin_arr = NULL;       /* array of lower twin primes         */
static size_t twin_size = 0;            /* number of twins stored             */
static size_t twin_cap = 0;             /* allocated capacity                 */

/* Set/get a bit in the sieve (number 3 corresponds to index 0) */
static inline void set_bit(size_t idx) { sieve_bits[idx >> 3] |= (1U << (idx & 7)); }
static inline bool get_bit(size_t idx) { return (sieve_bits[idx >> 3] >> (idx & 7)) & 1U; }

/* Rebuild sieve up to `limit` and regenerate twin list */
static void rebuild_sieve(idx_t limit)
{
    /* free previous data */
    free(sieve_bits);
    free(twin_arr);
    sieve_bits = NULL;
    twin_arr = NULL;
    twin_size = twin_cap = 0;

    /* allocate bitset for odd numbers ≥3 */
    size_t bits_needed = (limit >= 3 ? (limit - 3) / 2 + 1 : 0);
    sieve_bits_len = (bits_needed >> 3) + 1;
    sieve_bits = (uint8_t *)malloc(sieve_bits_len);
    if (!sieve_bits) abort();
    memset(sieve_bits, 0, sieve_bits_len);

    /* Sieve odds */
    idx_t sqrt_limit = (idx_t)(sqrt((long double)limit) + 0.5);
    for (idx_t i = 3; i <= sqrt_limit; i += 2) {
        size_t idx_i = (i - 3) >> 1;          /* index for number i */
        if (!get_bit(idx_i)) {
            for (idx_t j = i * i; j <= limit; j += i << 1) {
                size_t idx_j = (j - 3) >> 1;
                set_bit(idx_j);
            }
        }
    }

    /* Build twin lower-prime list */
    twin_cap = 1024;
    twin_arr = (uint32_t *)malloc(twin_cap * sizeof(uint32_t));
    if (!twin_arr) abort();

    for (idx_t p = 3; p + 2 <= limit; p += 2) {
        size_t idx_p = (p - 3) >> 1;
        size_t idx_p2 = (p + 2 - 3) >> 1;
        if (!get_bit(idx_p) && !get_bit(idx_p2)) {
            if (twin_size == twin_cap) {
                twin_cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(twin_arr, twin_cap * sizeof(uint32_t));
                if (!tmp) abort();
                twin_arr = tmp;
            }
            twin_arr[twin_size++] = (uint32_t)p;
        }
    }

    sieve_limit = limit;
}

/* Binary search for number of twins ≤ lim */
static size_t count_twin_leq(idx_t lim)
{
    size_t lo = 0, hi = twin_size;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if ((idx_t)twin_arr[mid] <= lim)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Ensure we have a sieve covering n² */
    idx_t max_limit = n * n;
    if (max_limit > sieve_limit) rebuild_sieve(max_limit);

    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t limit_k = k * n;
        size_t q = count_twin_leq(limit_k);
        if (q >= 2 &&
            is_prime_small((seq_t)q) &&
            is_prime_small((seq_t)(q + 2)))
            ++result;
    }
    return result;
}
