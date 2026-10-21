/*
 * A054640  a(n) is the sum of the divisors of the n-th primorial: a(n) =
 *          A000203(A002110(n)).
 * https://oeis.org/A054640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=0}^{n‑1} (prime_i + 1), with a(0) = 1 (recurrence:
 *            a(n+1) = a(n) * (prime_n + 1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/build_primes_upto.h"

/* ---------- prime generation state ----------------------------------- */
static uint64_t *g_primes   = NULL;   /* array of prime numbers */
static size_t   g_prime_cnt = 0;      /* number of primes stored   */
static bool     primes_ready = false;

/* initialise prime list sufficient for n up to 10 000
   (200 000 > the 10 000‑th prime 104 729). */
static void init_primes(void)
{
    if (primes_ready) return;
    const idx_t LIMIT = 200000;
    build_primes_upto(LIMIT, &g_primes, &g_prime_cnt);
    primes_ready = true;
}

/* ---------- memoised a(n) state ------------------------------------- */
static seq_t *a_cache        = NULL;   /* cache of already computed terms */
static size_t a_cache_cap    = 0;      /* allocated elements in a_cache   */
static idx_t  a_last         = 0;      /* largest n such that a_cache[n] is valid */

/* ensure that a_cache has entries 0..n and the missing values are computed. */
static void ensure_a_cache(idx_t n)
{
    if (n <= a_last) return;                /* already computed */

    init_primes();                           /* primes available */

    /* make a_cache large enough */
    size_t needed = (size_t)n + 1;           /* indices 0..n inclusive */
    if (needed > a_cache_cap) {
        size_t new_cap = a_cache_cap == 0 ? 1 : a_cache_cap * 2;
        while (new_cap < needed) new_cap *= 2;
        a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache_cap = new_cap;
    }

    /* initialise a(0) once */
    a_cache[0] = (seq_t)1;
    a_last = 0;

    /* compute values up to n using the recurrence */
    for (idx_t i = a_last + 1; i <= n; ++i) {
        seq_t factor = ((seq_t)g_primes[i - 1]) + 1;   /* prime_i-1 + 1 */
        a_cache[i] = a_cache[i - 1] * factor;
    }
    a_last = n;
}

/* --------- sequence function for A054640 ------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_a_cache(n);
    return a_cache[n];
}
