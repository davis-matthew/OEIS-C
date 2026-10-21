/*
 * A206589  Number of solutions (n,k) of p(k+1)=p(n+1) (mod n), where 1<=k<n.
 * https://oeis.org/A206589
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k : 1 <= k < n , p(k+1) ≡ p(n+1) (mod n) }|, where
 *            p(m) is the m‑th prime
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine uses an incremental sieve: it keeps a cache of all primes
   found up to the greatest limit needed so far.  For each invocation
   a(idx_t n) the cache is enlarged if necessary, then the required
   primes are accessed directly from the array.  The computation is
   O(n log log n) per call, which is well below the time limit for n
   up to about 10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* --- Cache of primes -------------------------------------------------- */

static uint64_t *prime_cache = NULL;   /* primes_cache[i] = p(i+1) (0‑based) */
static size_t prime_cache_cnt = 0;      /* number of primes stored */
static uint64_t prime_cache_limit = 0;  /* highest number sieved */

/* Estimate an upper bound for the m‑th prime (m >= 1).  For m >= 6 the
   well known bound p_m <= m(log m + log log m) holds.  For very small
   m we return a constant. */
static uint64_t estimate_upper_limit(idx_t m)
{
    if (m < 6) return 15;
    double dm = (double)m;
    double lim = dm * (log(dm) + log(log(dm))) + 3.0;
    if (lim > (double)UINT64_MAX - 10) lim = (double)UINT64_MAX - 10;
    return (uint64_t)lim;
}

/* Ensure that the cache contains primes up to at least the given
   limit (i.e. includes all primes <= limit).  If the cache already
   covers the limit it returns immediately.  The sieve fills the cache
   from scratch up to the new limit; this simple approach is fast
   enough for the intended range of n. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= prime_cache_limit) return;

    /* Allocate array for marking composites.  In C++ the result of
       calloc must be explicitly cast; C allows implicit conversion. */
    char *is_comp = (char *)calloc((size_t)(limit + 1), sizeof(char));
    if (!is_comp) exit(1);

    size_t new_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            ++new_cnt;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    uint64_t *new_primes = (uint64_t *)malloc(new_cnt * sizeof(uint64_t));
    if (!new_primes) exit(1);

    size_t pos = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) new_primes[pos++] = i;
    }

    free(is_comp);

    if (prime_cache) free(prime_cache);
    prime_cache      = new_primes;
    prime_cache_cnt  = new_cnt;
    prime_cache_limit = limit;
}

/* --- Main sequence function ------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* offset starts at 2 */

    /* We need the (n+1)‑th prime and all primes with indices
       1 … n (i.e. primes_arr[0 .. n]).  Compute an upper bound
       for the required prime. */
    idx_t target_index = n + 1;          /* m = n+1 for p(m) */
    uint64_t needed_limit = estimate_upper_limit(target_index);

    ensure_primes_up_to(needed_limit);

    /* Sanity check: enough primes must now be available.  If not,
       the bound was too low; we retry with a larger bound. */
    while (prime_cache_cnt < target_index) {
        needed_limit = estimate_upper_limit(target_index + 10);
        ensure_primes_up_to(needed_limit);
    }

    /* Compute the target residue r = p(n+1) mod n. */
    uint64_t target_r = prime_cache[n] % n;   /* prime_cache[n] == p(n+1) */

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        if ((prime_cache[k] % n) == target_r) ++count;
    }

    return count;
}
