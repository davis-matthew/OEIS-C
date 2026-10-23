/*
 * A166712  Number of primes in (n*log(n)..(n+1)*log(n+1)] semi-open
 *          intervals, n >= 1.
 * https://oeis.org/A166712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi((n+1)*log(n+1)) - pi(n*log(n))
 * Range:     u8 n <= 8365, u16 n <= 8365, u32 n <= 8365, u64 n <= 8365,
 *            u128 n <= 8365, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static storage for primes */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static size_t primes_cap = 0;
    static idx_t max_built_n = 0;

    if (n == 0) return 0;            /* defined only for n ≥ 1 */

    /* Rebuild the prime table if we need a larger maximum */
    if (n > max_built_n) {
        /* Upper bound for the largest interval needed */
        double limit_f = (double)(n + 1) * log((double)(n + 1));
        idx_t limit = (idx_t)(limit_f) + 10;      /* safety margin */

        /* Free old table if it exists */
        free(primes);
        primes = NULL;
        primes_cnt = 0;
        primes_cap = 0;

        /* Simple sieve of Eratosthenes up to 'limit' */
        bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
        if (!is_prime) return 0;        /* allocation failure */

        for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
        if (limit >= 0) is_prime[0] = false;
        if (limit >= 1) is_prime[1] = false;

        for (idx_t p = 2; (size_t)p * p <= (size_t)limit; ++p) {
            if (is_prime[p]) {
                for (idx_t mult = p * p; mult <= limit; mult += p) {
                    is_prime[mult] = false;
                }
            }
        }

        /* Rough estimate of number of primes for allocation */
        size_t est = (size_t)(limit / (log((double)limit) * 1.2)) + 10;
        if (est < 10) est = 10;

        primes = (uint64_t*)malloc(est * sizeof(uint64_t));
        if (!primes) { free(is_prime); return 0; }
        primes_cnt = 0;
        primes_cap = est;

        for (idx_t i = 2; i <= limit; ++i) {
            if (is_prime[i]) {
                if (primes_cnt >= primes_cap) {
                    primes_cap *= 2;
                    uint64_t *tmp = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
                    if (!tmp) { free(is_prime); free(primes); return 0; }
                    primes = tmp;
                }
                primes[primes_cnt++] = (uint64_t)i;
            }
        }

        free(is_prime);
        max_built_n = n;
    }

    /* Compute the two bounds of the interval */
    double start_val = (double)n * log((double)n);
    double end_val   = (double)(n + 1) * log((double)(n + 1));

    uint64_t start_int = (uint64_t)start_val;  /* floor of n*log(n) */
    uint64_t end_int   = (uint64_t)end_val;    /* floor of (n+1)*log(n+1) */

    /* Count primes ≤ start_int and ≤ end_int */
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= start_int) lo = mid + 1;
        else                          hi = mid;
    }
    size_t count_start = lo;  /* primes ≤ start_int */

    lo = 0; hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= end_int) lo = mid + 1;
        else                         hi = mid;
    }
    size_t count_end = lo;    /* primes ≤ end_int */

    seq_t result = (seq_t)(count_end - count_start);
    return result;
}
