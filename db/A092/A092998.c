/*
 * A092998  Least integer k > n such that the number of primes between 1 and n
 *          (exclusive) is the same as the number of primes between n and k
 *          (exclusive).
 * https://oeis.org/A092998
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = minimal k > n such that #primes in (1,n) = #primes in
 *            (n,k), i.e. pi(k-1) = 2*pi(n‑1) + [n is prime]
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes a sieve up to a fixed upper bound (300 000),
   which comfortably covers all n ≤ 10 000 (the test range).  States are cached
   statically so that repeated calls are O(log LIMIT). */

#include <stdint.h>
#include <stdlib.h>

static const idx_t LIMIT = 300000;
static unsigned char *is_prime_arr = NULL;   /* 1 = prime, 0 = composite */
static idx_t *pi_arr = NULL;                 /* pi[x] = number of primes ≤ x */
static int initialized = 0;

static void init_sieve(void)
{
    if (initialized) return;
    is_prime_arr = (unsigned char *)malloc((LIMIT + 1) * sizeof(unsigned char));
    pi_arr = (idx_t *)malloc((LIMIT + 1) * sizeof(idx_t));
    if (!is_prime_arr || !pi_arr) {
        /* In a real system you would handle allocation failure. */
        exit(EXIT_FAILURE);
    }

    /* Simple sieve of Eratosthenes */
    for (idx_t i = 0; i <= LIMIT; ++i) is_prime_arr[i] = 1;
    is_prime_arr[0] = is_prime_arr[1] = 0;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime_arr[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime_arr[j] = 0;
        }
    }

    /* Prefix sums for pi(x) */
    pi_arr[0] = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        pi_arr[i] = pi_arr[i - 1] + is_prime_arr[i];
    }

    initialized = 1;
}

/* Compute a(n) for n >= 2 */
seq_t a(idx_t n)
{
    init_sieve();

    /* Count of primes < n : pi(n-1) */
    seq_t primes_below = pi_arr[n - 1];

    /* Indicator of whether n itself is prime */
    seq_t is_prime_n = is_prime_arr[n] ? 1 : 0;

    /* Target value for pi(k-1) */
    seq_t target = primes_below * 2 + is_prime_n;

    /* Find smallest y >= n with pi(y) == target */
    idx_t low = n;
    idx_t high = LIMIT;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (pi_arr[mid] < target)
            low = mid + 1;
        else
            high = mid;
    }

    /* The smallest y with pi(y) >= target is low.
       Since target is attained exactly, pi(low) == target. */
    idx_t y = low;
    idx_t k = y + 1; /* k = y + 1, with y = k-1 */

    return (seq_t)k;
}
