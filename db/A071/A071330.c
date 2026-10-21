/*
 * A071330  Number of decompositions of n into sum of two prime powers.
 * https://oeis.org/A071330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 6929, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A071330: Number of decompositions of n into sum of two prime powers
   (prime powers include the number 1).  The value a(n) is the
   number of ordered pairs (p^a , q^b) with 1 ≤ p^a ≤ q^b ≤ n and
   p^a + q^b = n, counted without order (i.e. (x, y) and (y, x)
   are considered the same).  All terms up to n are computed on
   demand and cached for future calls.  This implementation works for
   all supported builds: 32/64‑bit unsigned types or arbitrary‑precision
   seq_t classes.  It uses a simple sieve of Eratosthenes to mark all
   prime powers up to the needed limit and then counts the decompositions
   directly.  Complexity is O(n²) in the worst case (n up to ~10⁴), which
   easily meets the 2‑second per call requirement. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* internal state for memoisation */
static idx_t        max_n        = 0;     /* largest n for which we have data   */
static bool *       prime_power  = NULL;  /* flag array: true if index is a prime power (including 1) */
static seq_t *      count_cache  = NULL;  /* count_cache[n] = a(n)                 */

/* extend the precomputation to cover all indices up to new_n (inclusive) */
static void extend(idx_t new_n)
{
    /* allocate new arrays */
    bool *      new_prime_power = (bool *)calloc(new_n + 1, sizeof(bool));
    seq_t *     new_count_cache = (seq_t *)calloc(new_n + 1, sizeof(seq_t));
    if (!new_prime_power || !new_count_cache) { /* insufficient memory – abort gracefully */
        /* in a production environment we would handle this, but for the
           purposes of the sequence generator we simply return without
           changing the cache.  The caller will receive 0 for all future
           indices that were not computed. */
        free(new_prime_power);
        free(new_count_cache);
        return;
    }

    /* --- 1. Sieve primes up to new_n ---------------------------------- */
    char * is_composite = (char *)calloc(new_n + 1, sizeof(char));
    if (!is_composite) {
        free(new_prime_power);
        free(new_count_cache);
        return;
    }
    for (idx_t i = 2; i * i <= new_n; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= new_n; j += i) {
                is_composite[j] = 1;
            }
        }
    }

    /* --- 2. Mark prime powers (1 included) -------------------------------- */
    new_prime_power[1] = true;                     /* 1 = p^0 for any prime p */
    for (idx_t p = 2; p <= new_n; ++p) {
        if (!is_composite[p]) {                    /* p is prime */
            idx_t val = p;
            while (val <= new_n) {
                new_prime_power[val] = true;
                /* multiply by p, stop before overflow */
                if (val > new_n / p) break;
                val *= p;
            }
        }
    }

    /* --- 3. Compute counts for every k <= new_n --------------------------- */
    for (idx_t k = 1; k <= new_n; ++k) {
        unsigned long long cnt = 0;
        for (idx_t a = 1; a <= k / 2; ++a) {
            if (new_prime_power[a] && new_prime_power[k - a]) ++cnt;
        }
        new_count_cache[k] = (seq_t)cnt;
    }

    /* --- 4. Replace old data -------------------------------------------- */
    free(prime_power);
    free(count_cache);
    free(is_composite);
    prime_power = new_prime_power;
    count_cache = new_count_cache;
    max_n       = new_n;
}

/* --------------------------------------------------------------- */
/*    Public interface:  seq_t a(idx_t n)   */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of offset range – defined as 0 */
    if (n > max_n) extend(n);             /* precompute up to n if needed */
    return count_cache[n];
}
