/*
 * A238224  Number of pairs {j, k} with 0 < j < k <= n and k == 1 (mod j) such
 *          that pi(j*n) divides pi(k*n), where pi(.) is given by A000720.
 * https://oeis.org/A238224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs {j,k} with 0 < j < k ≤ n, k ≡ 1 (mod j)
 *            and π(j·n) divides π(k·n), where π(.) is the prime–counting
 *            function
 * Range:     u8 n <= 41, u16 n <= 1133, u32 n <= 2900, u64 n <= 2900,
 *            u128 n <= 2900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* static cache of sieved data */
static idx_t _cached_n = 0;            /* size of the cache */
static uint8_t * _cached_bitset = NULL;/* bitset for composites, 1 = composite */
static seq_t * _cached_pi = NULL;      /* π(j·n) for j = 0.._cached_n */

/* bit operations on the composite bitset */
static inline void _set_composite(idx_t v) { _cached_bitset[v >> 3] |= 1U << (v & 7); }
static inline int  _is_composite(idx_t v) { return (_cached_bitset[v >> 3] >> (v & 7)) & 1; }

/* build a sieve up to n*n and store π(j·n) for j=1..n */
static void _build_pi(idx_t n)
{
    /* free old cache, if any */
    if (_cached_bitset) free(_cached_bitset);
    if (_cached_pi)    free(_cached_pi);

    _cached_n = n;
    uint64_t limit = (uint64_t)n * (uint64_t)n;
    size_t byte_len = (limit >> 3) + 1;
    _cached_bitset = (uint8_t*)calloc(byte_len, 1);     /* all bits 0 = prime only for 0,1 */

    /* sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!_is_composite(i)) {
            for (idx_t j = i * i; j <= limit; j += i)
                _set_composite(j);
        }
    }

    /* allocate array for π(j·n) values */
    _cached_pi = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t prime_count = 0;

    /* walk through numbers up to limit, count primes,
       and store the count at each multiple of n */
    for (idx_t num = 2; num <= limit; ++num) {
        if (!_is_composite(num))
            ++prime_count;
        if (num % n == 0)
            _cached_pi[num / n] = prime_count;    /* num = j·n, j = num/n */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > _cached_n) _build_pi(n);   /* build/extend cache as needed */

    seq_t count = 0;
    for (idx_t j = 1; j < n; ++j) {
        seq_t pi_j = _cached_pi[j];
        if (pi_j == 0) continue;       /* safety; occurs only for n=1 */
        for (idx_t t = 1;; ++t) {
            idx_t k = 1 + t * j;
            if (k > n) break;
            seq_t pi_k = _cached_pi[k];
            if (pi_k % pi_j == 0)
                ++count;
        }
    }
    return count;
}
