/*
 * A094741  Number of primes of the form k+1 where k is coprime to n, k < n.
 * https://oeis.org/A094741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p ≤ n such that gcd(p – 1, n) = 1
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
// This implementation builds a prime sieve up to the requested bound
// (incrementally) and counts the suitable primes for each call.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* static state for incremental prime generation */
static idx_t sieve_limit = 0;
static idx_t *sieve_primes = NULL;   /* array of primes up to sieve_limit */
static size_t sieve_count = 0;        /* number of primes stored */

/* ensure that all primes ≤ limit are available in sieve_primes */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= sieve_limit)
        return;          /* already enough */

    /* allocate new sieve */
    size_t sz = (size_t)(limit + 1);
    uint8_t *comp = (uint8_t*)calloc(sz, 1);   /* 0 = prime candidate */
    if (!comp) return;   /* out‑of‑memory, leave state unchanged */

    idx_t i;
    for (i = 2; i * i <= limit; ++i)
        if (!comp[i])
            for (idx_t j = i * i; j <= limit; j += i)
                comp[j] = 1;

    size_t cnt = 0;
    for (i = 2; i <= limit; ++i)
        if (!comp[i])
            ++cnt;

    idx_t *arr = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!arr) {
        free(comp);
        return;
    }

    size_t pos = 0;
    for (i = 2; i <= limit; ++i)
        if (!comp[i])
            arr[pos++] = i;

    free(sieve_primes);
    free(comp);

    sieve_primes = arr;
    sieve_count = cnt;
    sieve_limit = limit;
}

/* greatest common divisor for idx_t values */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* safety for n == 0 */

    ensure_primes_upto(n);
    if (!sieve_primes) return (seq_t)0;   /* failed allocation */

    size_t cnt = 0;
    for (size_t idx = 0; idx < sieve_count && sieve_primes[idx] <= n; ++idx) {
        idx_t p = sieve_primes[idx];
        if (p == 2) {
            /* k = 1 is always coprime to any n */
            if (n >= 2) ++cnt;
        } else {
            if (gcd_idx(p - 1, n) == 1)
                ++cnt;
        }
    }
    return (seq_t)cnt;
}
