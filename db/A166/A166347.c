/*
 * A166347  Irregular triangle read by rows of the frequency with which each
 *          value v of floor (j^2/p) occurs within A165974 for each prime p,
 *          taken over 1 <= j <= p - 1.
 * https://oeis.org/A166347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = #{ j : 1 ≤ j ≤ p-1 , floor(j²/p) = v }, where p is the
 *            prime for the row containing the n‑th term, and v = n – Σ_{i<p}
 *            primes[i] – 1
 * Range:     u8 n <= 8275, u16 n <= 8275, u32 n <= 8275, u64 n <= 8275,
 *            u128 n <= 8275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------------------------------------------------------------------------
   Standard headers
   ---------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------------
   Helper: integer square root (floor)
   ---------------------------------------------------------------------------- */
static inline idx_t floor_sqrt(idx_t n)
{
    if (n < 2) return n;                 /* 0 or 1 */
    idx_t low = 0, high = n, ans = 0;
    while (high >= low) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid <= n / mid) {            /* mid*mid <= n without overflow */
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Helper: integer square root (ceil) */
static inline idx_t ceil_sqrt(idx_t n)
{
    idx_t fs = floor_sqrt(n);
    return (fs * fs == n) ? fs : fs + 1;
}

/* ----------------------------------------------------------------------------
   Prime generation (sieve) with cumulative sums
   ---------------------------------------------------------------------------- */
static idx_t *prime_arr = NULL;     /* primes[0 ... prime_count-1]   */
static idx_t *cum_arr   = NULL;     /* cumulative sums              */
static size_t prime_count = 0;      /* number of primes stored      */
static idx_t  current_limit = 0;    /* current sieve upper bound    */

/* Ensure we have enough primes so that cum_arr[prime_count-1] ≥ target */
static void ensure_primes(idx_t target)
{
    if (prime_count > 0 && cum_arr[prime_count-1] >= target) return;

    idx_t new_limit = current_limit ? current_limit * 2 : 2;
    for (;;) {
        /* Allocate and build the sieve up to new_limit */
        bool *is_prime = (bool*)malloc((size_t)(new_limit + 1) * sizeof(bool));
        if (!is_prime) exit(1);
        for (idx_t i = 0; i <= new_limit; ++i) is_prime[i] = true;
        if (new_limit >= 0) is_prime[0] = false;
        if (new_limit >= 1) is_prime[1] = false;
        for (idx_t i = 2; i <= new_limit / i; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= new_limit; j += i) {
                    is_prime[j] = false;
                }
            }
        }

        /* Count primes and build arrays */
        size_t count = 0;
        for (idx_t i = 2; i <= new_limit; ++i)
            if (is_prime[i]) ++count;

        idx_t *new_primes = (idx_t*)malloc(count * sizeof(idx_t));
        idx_t *new_cum    = (idx_t*)malloc(count * sizeof(idx_t));
        if (!new_primes || !new_cum) exit(1);

        idx_t sum = 0;
        size_t pos = 0;
        for (idx_t i = 2; i <= new_limit; ++i) {
            if (is_prime[i]) {
                new_primes[pos] = i;
                sum += i;
                new_cum[pos] = sum;
                ++pos;
            }
        }
        free(is_prime);

        /* Replace old arrays */
        free(prime_arr);
        free(cum_arr);
        prime_arr = new_primes;
        cum_arr   = new_cum;
        prime_count = count;
        current_limit = new_limit;

        if (cum_arr[prime_count-1] >= target) break;
        new_limit *= 2;                            /* grow further if needed */
    }
}

/* Binary search to locate the row containing index n */
static inline size_t row_index(idx_t n)
{
    size_t lo = 0, hi = prime_count - 1;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (cum_arr[mid] >= n) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

/* ----------------------------------------------------------------------------
   Sequence function required by the build system
   ---------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                /* no valid terms for n<1 */

    ensure_primes(n);                          /* guarantees enough primes */

    size_t k = row_index(n);                   /* row number (prime index) */
    idx_t p = prime_arr[k];                    /* prime for this row */
    idx_t prev_sum = (k == 0) ? 0 : cum_arr[k-1];
    idx_t v = n - prev_sum - 1;                /* 0‑based offset in the row */

    /* Compute the number of j such that floor(j²/p) = v */
    idx_t low = ceil_sqrt(v * p);
    if (low < 1) low = 1;
    idx_t high = floor_sqrt((v + 1) * p - 1);
    if (high > p - 1) high = p - 1;

    if (high < low) return (seq_t)0;
    return (seq_t)(high - low + 1);
}
