/*
 * A215411  a(0) = 0; a(n+1) = 2*a(n) + k where k = 0 if prime(n+2)/prime(n+1)
 *          > prime(n+1)/prime(n), otherwise k = 1.
 * https://oeis.org/A215411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n>=1: a(n) = 2*a(n-1) + k, where k = 0 if
 *            prime(n+2)/prime(n+1) > prime(n+1)/prime(n), otherwise k = 1.
 *            (Prime indices start at 1: prime(1)=2)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a list of primes generated on demand
   and evaluates the recurrence iteratively.  It uses only the
   operations allowed on seq_t and idx_t.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime list (0‑based: primes[0] = 2).  */
static uint64_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

/* Integer square root (floor).  */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n, mid, best = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            best = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return best;
}

/* Ensure that primes[0 .. need-1] are populated.  */
static void ensure_primes(size_t need)
{
    if (primes_len >= need) return;
    if (primes_cap == 0) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    } else if (primes_cap < need) {
        primes_cap = need * 2;
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    }
    uint64_t candidate = primes_len ? primes[primes_len - 1] + 1 : 2;
    while (primes_len < need) {
        bool isprime = true;
        uint64_t r = isqrt_u64(candidate);
        for (size_t j = 0; j < primes_len && primes[j] <= r; ++j) {
            if (candidate % primes[j] == 0) { isprime = false; break; }
        }
        if (isprime) primes[primes_len++] = candidate;
        ++candidate;
    }
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t need = (size_t)n + 2;          /* need primes up to index n+2 */
    ensure_primes(need);
    seq_t val = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t p_n   = primes[i - 1];   /* prime(i)   */
        uint64_t p_np1 = primes[i];       /* prime(i+1) */
        uint64_t p_np2 = primes[i + 1];   /* prime(i+2) */
        uint64_t lhs = p_np2 * p_n;
        uint64_t rhs = p_np1 * p_np1;
        seq_t k = (lhs > rhs) ? (seq_t)0 : (seq_t)1;
        val = val * (seq_t)2 + k;
    }
    return val;
}
