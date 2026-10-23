/*
 * A198472  a(n)=q(n) if 4 | q(n)-2, and a(n)=q(n)/2 if 4 | q(n), where q(n)
 *          is the least practical number q>n with 2(n+1)-q practical.
 * https://oeis.org/A198472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = q(n) if 4 | q(n)-2 * a(n) = q(n)/2 if 4 | q(n) * * where
 *            q(n) is the least practical number q > n for which * 2(n+1)-q is
 *            practical. Practical numbers are tested using * the
 *            characterization via prime factors. The routine keeps * a cache
 *            of primes and practical numbers up to the required *...
 * Range:     u8 n <= 221, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/* Integer square root for unsigned 64‑bit integers.                     */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n >> 1U;
    if (n >= 4) hi += 2;          /* ensure hi^2 >= n for very small n */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) lo = mid + 1;
        else        hi = mid - 1;
    }
    return hi;                     /* hi^2 < n < (hi+1)^2 */
}

/* --------------------------------------------------------------------- */
/* Prime generation (trial division).                                    */
static uint64_t *primes_arr   = NULL;
static size_t   primes_len    = 0;
static size_t   primes_cap    = 0;
static idx_t    prime_last_checked = 1;

static void extend_primes_to(idx_t limit)
{
    if (limit <= prime_last_checked) return;

    for (idx_t cand = prime_last_checked + 1; cand <= limit; ++cand) {
        bool isprime = true;
        idx_t r = isqrt_u64(cand);
        for (size_t i = 0; i < primes_len && primes_arr[i] <= r; ++i) {
            if (cand % primes_arr[i] == 0) { isprime = false; break; }
        }
        if (isprime) {
            if (primes_len == primes_cap) {
                size_t ncap = primes_cap ? primes_cap << 1 : 16;
                uint64_t *newp = (uint64_t*)realloc(primes_arr, ncap * sizeof(uint64_t));
                if (!newp) exit(1);
                primes_arr = newp;
                primes_cap = ncap;
            }
            primes_arr[primes_len++] = cand;
        }
    }
    prime_last_checked = limit;
}

/* --------------------------------------------------------------------- */
/* Practical number test.  Uses the characterization via primes and sigma. */
static bool is_practical(idx_t n)
{
    if (n == 1) return true;

    uint64_t reach = 1;
    idx_t tmp = n;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes_arr[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            unsigned int exp = 0;
            while (tmp % p == 0) { tmp /= p; ++exp; }
            if (p > reach + 1) return false;
            /* sigma(p^exp) = 1 + p + ... + p^exp */
            uint64_t sigma = 1;
            uint64_t pow   = 1;
            for (unsigned int j = 0; j < exp; ++j) {
                pow   *= p;
                sigma += pow;
            }
            reach *= sigma;
        }
    }
    if (tmp > 1) {
        uint64_t p = tmp;
        if (p > reach + 1) return false;
        reach *= (1 + p);
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Storage of all practical numbers found so far.                        */
static idx_t *practical_arr   = NULL;
static size_t practical_len   = 0;
static size_t practical_cap   = 0;
static idx_t practical_limit  = 0;

static void extend_practical_to(idx_t limit)
{
    if (limit <= practical_limit) return;

    idx_t sqrt_lim = isqrt_u64(limit);
    extend_primes_to(sqrt_lim);

    for (idx_t cand = practical_limit + 1; cand <= limit; ++cand) {
        if (is_practical(cand)) {
            if (practical_len == practical_cap) {
                size_t ncap = practical_cap ? practical_cap << 1 : 16;
                idx_t *newp = (idx_t*)realloc(practical_arr, ncap * sizeof(idx_t));
                if (!newp) exit(1);
                practical_arr = newp;
                practical_cap = ncap;
            }
            practical_arr[practical_len++] = cand;
        }
    }
    practical_limit = limit;
}

/* Binary search whether val is present in the practical array. */
static bool practical_contains(idx_t val)
{
    size_t l = 0, r = practical_len;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if (practical_arr[m] < val) l = m + 1;
        else                         r = m;
    }
    return l < practical_len && practical_arr[l] == val;
}

/* --------------------------------------------------------------------- */
/* The sequence function.                                                 */
seq_t a(idx_t n)
{
    /* m = 2*(n+1) */
    idx_t m = (n + 1) * 2;

    /* Find all practical numbers up to m-1.  */
    extend_practical_to(m - 1);

    /* Locate the first practical > n.  */
    size_t idx = 0;
    while (idx < practical_len && practical_arr[idx] <= n) ++idx;

    idx_t q = 0;
    for ( ; idx < practical_len && practical_arr[idx] < m; ++idx) {
        idx_t cand = practical_arr[idx];
        idx_t rem  = m - cand;
        if (rem > 0 && practical_contains(rem)) {
            q = cand;
            break;
        }
    }
    /* By definition a(n) always exists for n ≥ 1.  */
    if (q % 4 == 2) return (seq_t)q;
    return (seq_t)(q / 2);
}
