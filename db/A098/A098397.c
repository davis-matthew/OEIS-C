/*
 * A098397  Number of primes that are not less than prime(n)-Log2(prime(n))
 *          and not greater than prime(n)+Log2(prime(n)), where Log2=A000523.
 * https://oeis.org/A098397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{p prime : prime(n)-⌊log₂(prime(n))⌋ ≤ p ≤
 *            prime(n)+⌊log₂(prime(n))⌋}
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds a list of primes on demand, then uses binary search to count
 * the primes in the interval around the n‑th prime. */

#include <stdbool.h>
#include <stdlib.h>

/* Static storage for primes (0‑based array, prime_list[0] = 2). */
static idx_t *prime_list   = NULL;  /* dynamic array of primes */
static size_t prime_cap    = 0;      /* allocated capacity (in elements) */
static size_t prime_cnt    = 0;      /* number of primes stored so far */
static idx_t  bound_checked = 1;    /* smallest integer > bound_checked is guaranteed to be checked for primality */

/* Ensure the prime array has room for at least 'new_cap' elements. */
static void grow_prime_array(size_t new_cap)
{
    if (new_cap <= prime_cap) return;
    size_t new_cap2 = prime_cap ? prime_cap * 2 : 16;
    if (new_cap2 < new_cap) new_cap2 = new_cap;  /* avoid overflow */
    idx_t *tmp = (idx_t *)realloc(prime_list, new_cap2 * sizeof(idx_t));
    if (!tmp) exit(1);   /* abort on allocation failure */
    prime_list = tmp;
    prime_cap = new_cap2;
}

/* Test whether 'candidate' is prime using the primes already stored. */
static bool is_prime_candidate(idx_t candidate)
{
    if (candidate < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > candidate / p) break;               /* p*p would overflow or exceed candidate */
        if (candidate % p == 0) return false;
    }
    return true;
}

/* Extend the prime list so that all primes ≤ 'up_to' are stored. */
static void ensure_primes_upto(idx_t up_to)
{
    if (up_to <= bound_checked) return;
    for (idx_t num = bound_checked + 1; num <= up_to; ++num) {
        if (is_prime_candidate(num)) {
            grow_prime_array(prime_cnt + 1);
            prime_list[prime_cnt++] = num;
        }
    }
    bound_checked = up_to;
}

/* Binary search helpers on the prime array. */
static size_t lower_bound_prime(idx_t value)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < value) lo = mid + 1;
        else hi = mid;
    }
    return lo;   /* first index with prime_list[idx] >= value */
}

static size_t upper_bound_prime(idx_t value)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= value) lo = mid + 1;
        else hi = mid;
    }
    return lo;   /* first index with prime_list[idx] > value */
}

/* Compute floor(log₂(x)) for x ≥ 1.  Returns an idx_t value. */
static inline idx_t floor_log2_uint64(idx_t x)
{
    /* __builtin_clzll is defined for unsigned long long; cast is safe. */
    return (idx_t)(63 - __builtin_clzll((unsigned long long)x));
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range – return 0 as a safe fallback */

    /* Ensure that the n‑th prime is present. */
    while (prime_cnt < n) {
        /* Continue testing integers after the current bound. */
        ++bound_checked;
        if (is_prime_candidate(bound_checked)) {
            grow_prime_array(prime_cnt + 1);
            prime_list[prime_cnt++] = bound_checked;
        }
    }

    /* Retrieve the n‑th prime (1‑based index). */
    idx_t pn = prime_list[n - 1];

    /* Window limits. */
    idx_t log2pn = floor_log2_uint64(pn);
    idx_t lo = pn > log2pn ? pn - log2pn : (idx_t)2;   /* at least 2 */
    idx_t hi = pn + log2pn;

    /* Ensure primes up to hi are known for the subsequent searches. */
    if (hi > bound_checked) ensure_primes_upto(hi);

    /* Count primes in [lo, hi] by binary search. */
    size_t first = lower_bound_prime(lo);
    size_t last  = upper_bound_prime(hi);   /* first index > hi */
    size_t cnt   = last > first ? last - first : 0;

    return (seq_t)cnt;
}
