/*
 * A178350  Semiprimes with both a prime number of 0's and a prime number of
 *          1's in their binary representations.
 * https://oeis.org/A178350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th semiprime whose binary representation contains
 *            a prime number of 0’s and a prime number of 1’s
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _isprime_small, bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/_isprime_small.h"

/* --------------- static cache for the sequence --------------- */
struct seq_cache_struct {
    seq_t *data;
    size_t size;
    size_t cap;
} seq_cache = {NULL, 0, 0};

/* --------------- prime infrastructure ------------------------ */
static uint64_t *prime_list = NULL;
static size_t prime_size = 0, prime_cap = 0;
static uint64_t prime_candidate = 2;         /* next integer to test for primality */

/* ensure that prime_list contains all primes <= limit */
static void _ensure_primes(uint64_t limit)
{
    for (; prime_candidate <= limit; ++prime_candidate) {
        bool isp = true;
        uint64_t r = (uint64_t)sqrt((long double)prime_candidate);
        for (size_t i = 0; i < prime_size; ++i) {
            uint64_t p = prime_list[i];
            if (p > r) break;
            if (prime_candidate % p == 0) { isp = false; break; }
        }
        if (isp) {
            if (prime_size == prime_cap) {
                prime_cap = prime_cap? prime_cap * 2 : 16;
                prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_size++] = prime_candidate;
        }
    }
}

/* test whether x is semiprime (product of exactly two primes,
   allowing equality) */
static bool _is_semiprime(uint64_t x)
{
    if (x < 4) return false;
    int count = 0;
    uint64_t t = x;
    for (size_t i = 0; i < prime_size; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > t) break;
        while (t % p == 0) {
            ++count;
            t /= p;
            if (count > 2) return false;
        }
    }
    if (t > 1) ++count;
    return count == 2;
}

/* popcount for any seq_t (iterative) */
static int _popcount_seq(seq_t x)
{
    int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

/* append a value to the cache, growing storage as needed */
static void _append_cache(seq_t val)
{
    if (seq_cache.size == seq_cache.cap) {
        seq_cache.cap = seq_cache.cap? seq_cache.cap * 2 : 16;
        seq_cache.data = (seq_t*)realloc(seq_cache.data, seq_cache.cap * sizeof(seq_t));
    }
    seq_cache.data[seq_cache.size++] = val;
}

/* ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, but guard against 0 */

    /* generate terms until we have at least n of them */
    if (seq_cache.size < n) {
        uint64_t current = seq_cache.size? (uint64_t)(seq_cache.data[seq_cache.size - 1]) + 1 : 2;
        while (seq_cache.size < n) {
            uint64_t sr = (uint64_t)sqrt((long double)current);
            _ensure_primes(sr);
            if (_is_semiprime(current)) {
                seq_t val = (seq_t)current;
                int ones  = _popcount_seq(val);
                int bits  = bitlen_seq_t(val);
                int zeros = bits - ones;
                if (_isprime_small(ones) && _isprime_small(zeros))
                    _append_cache(val);
            }
            ++current;
        }
    }
    return seq_cache.data[n - 1];
}
