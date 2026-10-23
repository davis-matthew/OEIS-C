/*
 * A182942  Ranks of primes when all odd prime powers p^j, for j>=1, are
 *          jointly ranked.
 * https://oeis.org/A182942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A182942
 *  Ranks of primes when all odd prime powers p^j, j>=1, are jointly ranked.
 *  a(n) is the position of the n-th odd prime in the sorted list of all
 *  odd prime powers (3,5,7,9,11,13,17,25,...) .
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/*--- global cache ---------------------------------------------------------*/
static idx_t  *primes       = NULL;    /* all primes, primes[0] == 2       */
static size_t  primes_cnt   = 0;       /* number of primes in the array   */
static seq_t  *prefix       = NULL;    /* prefix[i] = #odd prime powers <= i */
static idx_t   current_lim  = 0;       /* upper limit up to which tables are built */

/*--- helper: estimate an upper bound for the (n+1)-th prime -----------------*/
static idx_t estimate_limit(idx_t needed_primes)
{
    if (needed_primes < 6) return 15U;
    double nd = (double)needed_primes;
    double lim = nd * (log(nd) + log(log(nd))) + 3.0;
    return (idx_t)lim;
}

/*--- helper: pre‑compute primes and prefix array up to a needed index -------*/
static void precompute(idx_t n)
{
    /* we need n+1 primes (including 2) */
    idx_t          needed = n + 1;
    idx_t          limit  = estimate_limit(needed);

    if (limit <= current_lim && primes_cnt > n) return; /* already ok */

    /* free old data */
    free(primes);
    free(prefix);
    primes     = NULL;
    prefix     = NULL;
    current_lim = 0;

    /* sieve up to limit ----------------------------------------------- */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime) return;                 /* allocation failed */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* gather primes ----------------------------------------------- */
    size_t pcnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++pcnt;

    primes = (idx_t *)malloc(pcnt * sizeof(idx_t));
    if (!primes) {
        free(is_prime);
        return;
    }

    idx_t pidx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[pidx++] = i;

    primes_cnt = pcnt;
    free(is_prime);

    /* mark odd prime powers --------------------------------------------- */
    bool *is_ppower = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_ppower) return;
    memset(is_ppower, 0, (size_t)(limit + 1) * sizeof(bool));

    for (idx_t pi = 1; pi < primes_cnt; ++pi) {        /* skip 2 */
        idx_t q   = primes[pi];
        idx_t val = q;
        while (val <= limit) {
            is_ppower[val] = true;
            if (val > limit / q) break;
            val *= q;
        }
    }

    /* build prefix sums ---------------------------------------------- */
    prefix = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!prefix) {
        free(is_ppower);
        return;
    }

    prefix[0] = (seq_t)0;
    for (idx_t i = 1; i <= limit; ++i)
        prefix[i] = prefix[i - 1] + (is_ppower[i] ? (seq_t)1 : (seq_t)0);

    free(is_ppower);
    current_lim = limit;
}

/*--- required function ----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n = 0 */

    precompute(n);                       /* guarantees primes[n] exists */

    /* nth odd prime, 0‑based indices: primes[0] = 2, primes[1] = 3 ...   */
    seq_t prime_n = primes[n];           /* fits in idx_t */
    idx_t pos     = (idx_t)prime_n;       /* convert to idx_t for indexing */

    return prefix[pos];
}
