/*
 * A176811  Number of primes between 2*(lesser of n-th twin prime pair) and
 *          2*(greater of n-th twin prime pair).
 * https://oeis.org/A176811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS formula (definition): a(n) = # of primes q with
 * 2*A001359(n) < q < 2*A006512(n)
 *           = # of primes between 2*lower twin prime and 2*upper twin prime.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Global data for a simple prime sieve and twin prime list */
static bool   *prime_flag = NULL;   /* sieve flags (prime_flag[i] == true -> i is prime) */
static idx_t    sieve_limit = 0;    /* current sieve upper bound */
static idx_t  *twin_lower  = NULL;  /* lower prime of each twin‑prime pair */
static size_t   twin_cnt   = 0;     /* number of twin pairs found */

static void init_sieve(idx_t limit)
/* Build the prime sieve and twin list up to at least 'limit'.
 * The sieve array size is limit+1 (indices 0…limit). */
{
    if (limit <= sieve_limit) return;     /* already sufficient */

    /* Grow limit to a convenient power of two */
    if (!sieve_limit) sieve_limit = 2;
    while (sieve_limit < limit) sieve_limit <<= 1;

    /* Allocate and initialise the sieve flags */
    bool *flag = (bool *)calloc((size_t)(sieve_limit + 1), sizeof(bool));
    for (unsigned i = 2; i <= sieve_limit; ++i) flag[i] = true;
    for (idx_t i = 2; i * i <= sieve_limit; ++i)
        if (flag[i])
            for (idx_t j = i * i; j <= sieve_limit; j += i) flag[j] = false;

    /* Count primes to allocate an array */
    size_t pc = 0;
    for (idx_t i = 2; i <= sieve_limit; ++i) if (flag[i]) ++pc;
    idx_t *pl = (idx_t *)malloc(pc * sizeof(idx_t));
    size_t pi = 0;
    for (idx_t i = 2; i <= sieve_limit; ++i) if (flag[i]) pl[pi++] = i;

    /* Build the twin‑prime lower list */
    size_t tc = 0;
    for (size_t k = 1; k < pc; ++k)
        if (pl[k] - pl[k-1] == 2) ++tc;
    idx_t *tw = (idx_t *)malloc(tc * sizeof(idx_t));
    size_t ti = 0;
    for (size_t k = 1; k < pc; ++k)
        if (pl[k] - pl[k-1] == 2) tw[ti++] = pl[k-1];

    /* Liberate old data */
    free(prime_flag);
    free(twin_lower);

    /* Link new data */
    prime_flag = flag;
    twin_lower = tw;
    twin_cnt   = tc;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined below offset */

    /* Rough estimate: the nth twin prime is < 200 * n; we need
     * to cover at least 2*p+3, so use a slightly larger buffer. */
    idx_t limit = n * 200ULL + 100ULL;
    while (1) {
        init_sieve(limit);
        if (twin_cnt >= n) break;
        limit <<= 1;                    /* double until sufficient */
    }

    /* Retrieve the n‑th twin pair (0‑based index n-1) */
    idx_t p  = twin_lower[n - 1];
    idx_t a1 = 2 * p + 1;                /* smallest candidate in interval */
    idx_t a2 = 2 * p + 3;                /* second candidate */

    seq_t cnt = 0;
    if (a1 <= sieve_limit && prime_flag[a1]) ++cnt;
    if (a2 <= sieve_limit && prime_flag[a2]) ++cnt;
    return cnt;
}
