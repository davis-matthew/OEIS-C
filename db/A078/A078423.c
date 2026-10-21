/*
 * A078423  Group the natural numbers into clumps with increasing prime
 *          numbers of elements, then multiply the members of each clump.
 * https://oeis.org/A078423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of consecutive integers from * 1 +
 *            sum_{k=1}^{n-1} p_k up to sum_{k=1}^{n} p_k, * where p_k is the
 *            k‑th prime (2,3,5,7,…). * This is equivalent to a(n) =
 *            (Σ_{k=1}^{n} p_k)! / (Σ_{k=1}^{n-1} p_k)!. * The implementation
 *            generates primes on demand, stores previously * computed
 *            terms,...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* External linkage required by the test harness */
seq_t a(idx_t n)
{
    /* Caches and state that survive across calls */
    static idx_t   *prime_cache   = NULL;   /* primes[0] = 2, primes[1] = 3, … */
    static size_t   prime_cap     = 0;      /* allocated capacity for prime_cache  */
    static idx_t    primes_len    = 0;      /* number of primes stored           */

    static seq_t   *a_cache       = NULL;   /* a_cache[0] = a(1), a_cache[1] = a(2), … */
    static size_t   a_cap         = 0;      /* allocated capacity for a_cache     */
    static idx_t    cache_size    = 0;      /* number of terms computed so far    */

    static idx_t   cum_sum        = 0;      /* Σ_{k=1}^{m} p_k for last computed m */
    static idx_t   last_prime     = 1;      /* last prime that has been generated (1 before the first prime) */

    /* Handle the illegal case n==0 explicitly (undefined by OEIS) */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Grow caches as needed */
    if (cache_size < n) {
        /* Ensure prime_cache can hold at least n primes */
        if (prime_cap < n) {
            size_t new_cap = prime_cap ? prime_cap * 2 : 16;
            if (new_cap < n) new_cap = n;
            idx_t *tmp = (idx_t *)realloc(prime_cache, new_cap * sizeof(idx_t));
            if (!tmp) exit(1);
            prime_cache = tmp;
            prime_cap = new_cap;
        }

        /* Ensure a_cache can hold at least n terms */
        if (a_cap < n) {
            size_t new_cap = a_cap ? a_cap * 2 : 16;
            if (new_cap < n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            a_cache = tmp;
            a_cap = new_cap;
        }

        /* Generate missing primes and terms from cache_size+1 up to n */
        while (cache_size < n) {
            /* Generate next prime */
            idx_t cand = last_prime + 1;
            while (true) {
                bool is_prime = true;
                for (idx_t i = 0; i < primes_len; ++i) {
                    idx_t p = prime_cache[i];
                    if (p * p > cand) break;
                    if (cand % p == 0) { is_prime = false; break; }
                }
                if (is_prime) break;
                ++cand;
            }
            /* Store new prime */
            prime_cache[primes_len++] = cand;
            last_prime = cand;

            /* Compute a(next) = product of integers in the new block */
            idx_t start = cum_sum + 1;           /* first integer of the block   */
            idx_t end   = cum_sum + cand;        /* last  integer of the block   */
            seq_t prod  = 1;
            for (idx_t x = start; x <= end; ++x) {
                prod *= (seq_t)x;
            }
            a_cache[cache_size] = prod;
            cache_size++;
            cum_sum = end;
        }
    }

    /* Return the cached value */
    return a_cache[n - 1];
}
