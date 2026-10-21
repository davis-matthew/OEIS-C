/*
 * A205510  Binary Hamming distance between prime(n) and prime(n+1).
 * https://oeis.org/A205510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = binary Hamming distance between prime(n) and prime(n+1)
 * Range:     u8 n <= 53, u16 n <= 9486, u32 n <= 9486, u64 n <= 9486,
 *            u128 n <= 9486, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Assumes the following types are defined by the build:
   typedef ... seq_t;
   typedef ... idx_t;
*/

static uint32_t *prime_arr = NULL;
static size_t   prime_count = 0;
static size_t   sieve_limit = 0;

/* Integer floor of log2 for idx_t values. */
static inline size_t int_log2_uint64(idx_t v)
{
    size_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* Rebuild the sieve, storing all primes ≤ limit in prime_arr. */
static void recompute_sieve(size_t limit)
{
    uint8_t *composite = (uint8_t *)malloc(limit + 1);
    if (!composite) exit(1);
    memset(composite, 0, limit + 1);

    for (size_t i = 2; i * i <= limit; ++i)
        if (!composite[i])
            for (size_t j = i * i; j <= limit; j += i)
                composite[j] = 1;

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!composite[i]) ++cnt;

    uint32_t *arr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!arr) { free(composite); exit(1); }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!composite[i]) arr[idx++] = (uint32_t)i;

    free(composite);

    if (prime_arr) free(prime_arr);
    prime_arr   = arr;
    prime_count = cnt;
    sieve_limit = limit;
}

/* Ensure that at least (n+1) primes are available. */
static void ensure_primes(idx_t n)
{
    /* We need primes with indices 0 … n (i.e. n+1 primes). */
    if (prime_count >= (size_t)(n + 1)) return;

    size_t needed = (size_t)(n + 1);
    size_t limit;

    if (needed < 6) {
        limit = 15;
    } else {
        /* Conservative bound for the k‑th prime (k ≥ 6):
           p_k < k * (2·floor_log2(k) + 5). */
        size_t log2k = int_log2_uint64(needed);
        limit = needed * (log2k * 2 + 5);
        if (limit < sieve_limit * 2 + 100) limit = sieve_limit * 2 + 100;
    }
    if (limit < 100) limit = 100;
    recompute_sieve(limit);
}

/* Sequence term: binary Hamming distance between consecutive primes. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */
    ensure_primes(n);

    seq_t p1   = (seq_t)prime_arr[n - 1];
    seq_t p2   = (seq_t)prime_arr[n];
    seq_t diff = p1 ^ p2;

    unsigned int cnt = 0;
    while (diff != 0) {
        cnt++;
        diff &= diff - 1;
    }
    return (seq_t)cnt;
}
