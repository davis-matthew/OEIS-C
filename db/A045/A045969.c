/*
 * A045969  a(1)=6; if n = Product p_i^e_i, n>1, then a(n) = Product
 *          p_{i+1}^e_i * Product p_{i+2}^e_i.
 * https://oeis.org/A045969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045969
   a(1) = 6
   a(n>1) = ∏ p_i^{e_i} from n = ∏ p_i^{e_i}  →  ∏ p_{i+1}^{e_i} · ∏ p_{i+2}^{e_i}
   where p_i is the i‑th prime number in the prime list.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static bool _primes_ready = false;
static size_t _primes_cnt = 0;
/* enough to support primes up to 200000 (more than enough for n up to ~10⁴) */
#define _PRIMES_MAX 200000
static uint32_t _primes_buf[_PRIMES_MAX];

/* Simple sieve to fill _primes_buf */
static void _init_primes(void)
{
    if (_primes_ready) return;
    bool *sieve = (bool*)malloc((_PRIMES_MAX + 1) * sizeof(bool));
    if (!sieve) return; /* out of memory; fallback to empty list */
    for (int i = 0; i <= _PRIMES_MAX; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= _PRIMES_MAX; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= _PRIMES_MAX; j += i) sieve[j] = false;
        }
    }
    for (int i = 2; i <= _PRIMES_MAX; ++i) {
        if (sieve[i]) _primes_buf[_primes_cnt++] = (uint32_t)i;
    }
    free(sieve);
    _primes_ready = true;
}

/* binary search for prime index (0‑based) in _primes_buf */
static size_t _prime_index(uint64_t p)
{
    size_t lo = 0, hi = _primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (_primes_buf[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return lo;  /* assumes p is prime */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)6;

    _init_primes();

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* factorisation using trial primes */
    for (size_t pi = 0; pi < _primes_cnt && (uint64_t)_primes_buf[pi] * (uint64_t)_primes_buf[pi] <= m; ++pi) {
        uint32_t p = _primes_buf[pi];
        if (m % p != 0) continue;
        uint64_t exp = 0;
        while (m % p == 0) { m /= p; ++exp; }
        /* find next two primes */
        size_t idx = _prime_index(p);
        uint64_t p1 = _primes_buf[idx + 1];
        uint64_t p2 = _primes_buf[idx + 2];
        /* multiply result by p1^exp and p2^exp */
        for (uint64_t i = 0; i < exp; ++i) {
            result *= (seq_t)p1;
            result *= (seq_t)p2;
        }
    }
    if (m > 1) { /* remaining factor is prime */
        uint64_t p = m;
        uint64_t exp = 1;
        size_t idx = _prime_index(p);
        uint64_t p1 = _primes_buf[idx + 1];
        uint64_t p2 = _primes_buf[idx + 2];
        result *= (seq_t)p1;
        result *= (seq_t)p2;
    }

    return result;
}
