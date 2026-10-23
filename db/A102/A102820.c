/*
 * A102820  Number of primes between 2*prime(n) and 2*prime(n+1), where
 *          prime(n) is the n-th prime.
 * https://oeis.org/A102820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primes x such that 2*prime(n) < x <
 *            2*prime(n+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* External types supplied by the build system (do not re‑define) */
/* typedefs for seq_t and idx_t are provided by the calling environment */

/* ---------- Prime sieve storage ---------- */
static uint64_t *prime_list = NULL;   /* 0‑based array of primes */
static size_t   prime_count = 0;      /* number of primes stored */
static uint64_t prime_limit = 0;      /* sieve upper bound used */

/* ---------- Utility functions ---------- */

/* Rosser’s asymptotic upper bound for the n‑th prime.  */
static uint64_t estimate_prime_upper_bound(uint64_t n)
{
    if (n < 6) return 15ULL;              /* first few primes are < 15 */
    double dn = (double)n;
    double est = dn * (log(dn) + log(log(dn))) + 10.0;
    if (est > (double)UINT64_MAX) est = (double)UINT64_MAX - 1.0;
    return (uint64_t)est;
}

/* Eratosthenes sieve of size limit (inclusive).  The resulting prime
   list replaces any previously stored list.                      */
static void sieve_primes(uint64_t limit)
{
    uint8_t *is_p = (uint8_t *)malloc((size_t)limit + 1);
    if (!is_p) exit(1);              /* OOM – hard failure – acceptable */
    memset(is_p, 1, (size_t)limit + 1);
    is_p[0] = is_p[1] = 0;

    uint64_t root = (uint64_t)sqrt((double)limit);
    for (uint64_t i = 2; i <= root; ++i) {
        if (is_p[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) is_p[j] = 0;
        }
    }

    /* Count primes to allocate array */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_p[i]) ++cnt;

    uint64_t *new_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_list) { free(is_p); exit(1); }
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_p[i]) new_list[idx++] = i;

    free(is_p);
    free(prime_list);          /* old list may be NULL */
    prime_list = new_list;
    prime_count = cnt;
    prime_limit = limit;
}

/* Binary search: first index with arr[i] > val */
static size_t upper_bound(const uint64_t *arr, size_t n, uint64_t val)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] <= val) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

/* Ensure that at least 'need' primes are available and that the
   sieve covers at least 'min_limit'.  If not, rebuild the sieve.   */
static void ensure_primes(uint64_t need, uint64_t min_limit)
{
    if (prime_count >= need && prime_limit >= min_limit) return;

    uint64_t new_limit = prime_limit > min_limit ? prime_limit : min_limit;
    if (prime_count < need) {
        uint64_t est = estimate_prime_upper_bound(need);
        if (est > new_limit) new_limit = est;
    }
    sieve_primes(new_limit);
}

/* ---------- The requested sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* OEIS offset 1 – behaviour below 1 is irrelevant */

    /* We require prime(n) and prime(n+1) -> at least n+1 primes (0‑based) */
    ensure_primes((uint64_t)n + 1, 0);

    uint64_t p_n   = prime_list[(uint64_t)n - 1];   /* prime(n)   (1‑based) */
    uint64_t p_np1 = prime_list[(uint64_t)n];      /* prime(n+1) */

    uint64_t lower = 2 * p_n;
    uint64_t upper = 2 * p_np1;

    /* Need primality information up to upper‑1 */
    if (upper > prime_limit) ensure_primes(0, upper);

    /* Count primes > lower and < upper (i.e., <= upper-1) */
    size_t ub_upper = upper_bound(prime_list, prime_count, upper - 1);
    size_t ub_lower = upper_bound(prime_list, prime_count, lower);
    size_t count = ub_upper - ub_lower;

    return (seq_t)count;
}
