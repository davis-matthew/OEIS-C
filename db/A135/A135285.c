/*
 * A135285  Sum of staircase twin primes according to the rule: top * bottom -
 *          next top.
 * https://oeis.org/A135285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tl(n) * tu(n) - tl(n+1) where tl(k) is the lower twin of
 *            the k‑th twin prime pair and tu(k) = tl(k) + 2. (OEIS A135285)
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* seq_t and idx_t are supplied by the build system.  */
static seq_t *twin_prime = NULL;   /* array of lower twins, 0‑based */
static size_t twin_count   = 0;     /* number of stored lower twins   */
static idx_t  sieve_limit  = 0;     /* last limit used for sieve     */

/* Build twin primes up to the given limit (inclusive).   */
static void build_twin_primes_upto(idx_t limit)
{
    /* Allocate a simple prime flag array.  */
    unsigned char *is_prime = (unsigned char *)malloc((size_t)(limit + 1));
    if (!is_prime)
        exit(1);                       /* unlikely, but safe          */

    memset(is_prime, 1, (size_t)(limit + 1));
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    /* Sieve of Eratosthenes (using idx_t loops). */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;
        }
    }

    /* Count twin primes first to know capacity. */
    size_t count = 0;
    for (idx_t i = 2; i + 2 <= limit; ++i)
        if (is_prime[i] && is_prime[i + 2])
            ++count;

    /* Allocate or grow the twin array. */
    size_t new_cap = (twin_prime == NULL) ? count : twin_count + count;
    twin_prime = (seq_t *)realloc(twin_prime, new_cap * sizeof(seq_t));
    if (!twin_prime)
        exit(1);

    /* Store lower twins. */
    for (idx_t i = 2; i + 2 <= limit; ++i) {
        if (is_prime[i] && is_prime[i + 2]) {
            twin_prime[twin_count++] = (seq_t)i;
        }
    }

    free(is_prime);
    sieve_limit = limit;
}

/* Ensure we have at least (n+1) lower twins stored.   */
static void ensure_twin_primes(idx_t n)
{
    if (twin_prime && twin_count >= n + 1)
        return;                           /* enough already           */

    /* Choose an initial upper limit.  */
    idx_t limit = (sieve_limit) ? sieve_limit : 2000000ULL;
    if (twin_prime && twin_count <= n)
        limit = sieve_limit * 2;

    /* Keep doubling until we have enough twins.   */
    for (;;) {
        build_twin_primes_upto(limit);
        if (twin_count >= n + 1)
            break;
        limit *= 2;
    }
}

/* ---------------------------------------------------------- */
/* Sequence function required by the build.                   */
/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(1) is first term; n==0 is unused but handled gracefully. */
    if (n == 0)
        return (seq_t)0;

    /* Make sure we have the needed twin primes. */
    ensure_twin_primes(n);

    /* Retrieve lower twin of the current pair and the next pair. */
    seq_t low_cur      = twin_prime[n - 1];
    seq_t low_next     = twin_prime[n];

    /* Compute tu(n) = low_cur + 2.  */
    seq_t top = low_cur + (seq_t)2;

    /* A135285: low_cur * top - low_next. */
    return (low_cur * top) - low_next;
}
