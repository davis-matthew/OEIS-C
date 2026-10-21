/*
 * A081738  a(n) = Sum_{2 <= p <= n, p prime} p^2.
 * https://oeis.org/A081738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p prime, 2 <= p <= n} p^2 (offset 1)
 * Range:     u8 n <= 12, u16 n <= 96, u32 n <= 4728, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a growing list of primes and the prefix
   sums of their squares.  Each call expands the list up to the
   requested n, then returns the last sum whose prime is <= n.  */
#include <stdlib.h>

static idx_t *primes = NULL;          /* list of primes found so far        */
static size_t primes_len = 0, primes_cap = 0;
static seq_t *prime_sqsums = NULL;    /* prefix sums of p^2                 */
static size_t sums_len = 0, sums_cap = 0;
static idx_t max_checked = 1;         /* largest number that has been tested */

/* Grow the prime and sum arrays if the current capacity is insufficient. */
static void grow_if_needed(size_t need)
{
    if (need > primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        primes = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        primes_cap = new_cap;
    }
    if (need > sums_cap) {
        size_t new_cap = sums_cap ? sums_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        prime_sqsums = (seq_t *)realloc(prime_sqsums, new_cap * sizeof(seq_t));
        sums_cap = new_cap;
    }
}

/* Extend the prime list and the prefix sums up to at least n. */
static void ensure_primes_up_to(idx_t n)
{
    if (n <= max_checked) return;

    for (idx_t m = max_checked + 1; m <= n; ++m) {
        int is_prime = 1;
        for (size_t i = 0; i < primes_len; ++i) {
            idx_t p = primes[i];
            if (p * p > m) break;
            if (m % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            grow_if_needed(primes_len + 1);
            primes[primes_len++] = m;
            seq_t sq = (seq_t)m * m;
            seq_t sum = (sums_len > 0 ? prime_sqsums[sums_len - 1] : (seq_t)0) + sq;
            prime_sqsums[sums_len++] = sum;
        }
    }
    max_checked = n;
}

/* Return a(n) */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    ensure_primes_up_to(n);

    /* Binary search for the largest prime <= n */
    size_t lo = 0, hi = primes_len;
    size_t pos = (size_t)-1;  /* will become ns digits error? No.  */
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (primes[mid] <= n) {
            pos = mid;
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    if (pos == (size_t)-1) return (seq_t)0;
    return prime_sqsums[pos];
}
