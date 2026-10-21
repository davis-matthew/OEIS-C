/*
 * A056813  Largest non-unitary prime factor of LCM(1,...,n); that is, the
 *          largest prime which occurs to power > 1 in prime factorization of
 *          LCM(1,..,n).
 * https://oeis.org/A056813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A056813 – largest non‑unitary prime factor of LCM(1,…,n)
 *       a(n) = largest prime ≤ ⌊√n⌋  (with 1 treated as a prime).
 * This implementation builds a prime table up to the required limit and
 * performs a binary search for the answer.
 */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/_floor_isqrt_u64.h"

static uint64_t *prime_tbl = NULL;   /* dynamic array of primes */
static size_t prime_cap = 0;         /* allocated size of prime_tbl         */
static size_t prime_len = 0;         /* current number of primes in table   */
static uint64_t prime_limit = 0;     /* largest number for which primes are known */

/* build primes up to 'limit' (inclusive) using Eratosthenes sieve */
static void build_primes_upto(uint64_t limit) {
    if (limit <= prime_limit) return;

    size_t sz = (size_t)limit + 1;
    bool *sieve = (bool *)malloc(sz);
    if (!sieve) return;  /* allocation failed – caller will get 1 anyway */

    for (size_t i = 0; i < sz; ++i) sieve[i] = true;
    if (sz > 0) sieve[0] = false;
    if (sz > 1) sieve[1] = false;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) sieve[j] = false;
        }
    }

    /* count primes to know new capacity */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i) if (sieve[i]) ++count;

    if (count > prime_cap) {
        uint64_t *tmp = (uint64_t *)realloc(prime_tbl, count * sizeof(uint64_t));
        if (!tmp) { free(sieve); return; } /* on failure keep old table */
        prime_tbl = tmp;
        prime_cap = count;
    }

    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) prime_tbl[pos++] = (uint64_t)i;
    prime_len = pos;
    prime_limit = limit;
    free(sieve);
}

/* largest prime <= x (or 1 if none). x ≥ 0 */
static uint64_t largest_prime_le(uint64_t x) {
    if (x < 2) return 1;         /* only "prime" 1 allowed for small n */
    if (x > prime_limit) build_primes_upto(x);
    /* binary search in prime_tbl */
    size_t lo = 0, hi = prime_len - 1;
    uint64_t ans = 1;
    while (lo <= hi) {
        size_t mid = lo + (hi - lo) / 2;
        uint64_t p = prime_tbl[mid];
        if (p <= x) {
            ans = p;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* For n < 2 the sqrt is 0 or 1 → answer is 1 */
    if (n < 2) return 1;

    uint64_t r = _floor_isqrt_u64(n);          /* ⌊√n⌋ */
    uint64_t p = largest_prime_le(r);          /* largest prime ≤ r, or 1 */
    return (seq_t)p;
}
