/*
 * A062049  Integer part of geometric mean of first n primes.
 * https://oeis.org/A062049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ (p₁·p₂·…·pₙ)^(1/n) ⌋ , where pᵢ is the i‑th prime
 * Range:     u8 n <= 148, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 87 terms through n = 968 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Prime sieve up to a given limit.
   The sieve is rebuilt only when a larger limit is requested.
   Static storage is used so calls need not be monotonic in n. */
static void ensure_primes_upto(idx_t limit)
{
    static idx_t max_sieved = 0;
    static uint32_t *primes = NULL;
    static size_t primes_cnt = 0;

    if (limit <= max_sieved) return;

    /* allocate a sieve array */
    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(sz);
    for (size_t i = 0; i < sz; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* count primes */
    size_t new_cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++new_cnt;

    /* rebuild array */
    uint32_t *new_primes = (uint32_t *)realloc(primes, new_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_primes[idx++] = (uint32_t)i;

    free(is_prime);

    primes = new_primes;
    primes_cnt = new_cnt;
    max_sieved = limit;
}

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    /* Prepare prime table up to n */
    ensure_primes_upto(n);

    /* Sum of logarithms of the first n primes (long double for high precision) */
    long double log_prod = 0.0L;
    for (idx_t i = 1; i <= n; ++i) {
        long double p = (long double)get_prime(i);
        log_prod += logl(p);          /* natural logarithm (base e) */
    }

    /* Binary search on the integer m satisfying m^n ≤ product < (m+1)^n */
    idx_t low = 1;
    idx_t high = (idx_t)get_prime(n);     /* pₙ > a(n) for all n ≥ 1 */

    /* Small epsilon to guard against floating‑point rounding */
    const long double eps = 1e-14L;

    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        long double mid_log = logl((long double)mid);
        long double lhs = (long double)n * mid_log;
        if (lhs <= log_prod + eps)          /* mid^n ≤ product  */
            low = mid;
        else                                /* mid^n > product  */
            high = mid - 1;
    }

    return (seq_t)low;
}
