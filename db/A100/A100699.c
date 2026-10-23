/*
 * A100699  Number of ways to partition n into two squarefree numbers that are
 *          not prime.
 * https://oeis.org/A100699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count unordered pairs of squarefree composite numbers summing to n */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global state for optional memoisation / sieve */
static idx_t global_max = 0;          /* largest n for which we have data */
static bool *prime_flag = NULL;       /* prime_flag[i] == true iff i is prime   */
static bool *squarefree_flag = NULL;  /* squarefree_flag[i] == true iff i is square‑free   */
static idx_t *prime_list = NULL;      /* list of primes up to global_max        */
static idx_t prime_count = 0;         /* number of primes in prime_list         */

/* Rebuild all tables up to n.  Called only when a larger n is requested. */
static void recompute(idx_t n)
{
    /* Free existing data if any */
    if (prime_flag)      free(prime_flag);
    if (squarefree_flag) free(squarefree_flag);
    if (prime_list)      free(prime_list);

    /* Allocate new tables */
    prime_flag        = (bool *)calloc((size_t)(n + 1), sizeof(bool));
    squarefree_flag   = (bool *)calloc((size_t)(n + 1), sizeof(bool));
    if (!prime_flag || !squarefree_flag) {
        /* Memory failure – leave global state unchanged. */
        return;
    }

    /* Initialise primality flags – assume prime except 0,1 */
    prime_flag[0] = prime_flag[1] = false;
    for (idx_t i = 2; i <= n; ++i) prime_flag[i] = true;

    /* Initialise square‑free flags – assume square‑free, override later */
    for (idx_t i = 0; i <= n; ++i) squarefree_flag[i] = true;
    squarefree_flag[0] = false;   /* 0 is neither square‑free nor used */

    /* Sieve to determine primes */
    for (idx_t i = 2; (idx_t)(i * i) <= n; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                prime_flag[j] = false;
        }
    }

    /* Build list of primes */
    prime_count = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (prime_flag[i]) ++prime_count;
    prime_list = (idx_t *)malloc((size_t)prime_count * sizeof(idx_t));
    if (!prime_list) {
        free(prime_flag);
        free(squarefree_flag);
        prime_flag = squarefree_flag = NULL;
        return;
    }
    idx_t k = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (prime_flag[i]) prime_list[k++] = i;

    /* Mark numbers that are NOT square‑free (multiples of p²) */
    for (idx_t idx = 0; idx < prime_count; ++idx) {
        idx_t p  = prime_list[idx];
        idx_t p2 = p * p;
        if (p2 > n) break;
        for (idx_t j = p2; j <= n; j += p2)
            squarefree_flag[j] = false;
    }

    global_max = n;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* Ensure tables cover this n */
    if (n > global_max)
        recompute(n);

    seq_t count = 0;
    idx_t limit = n / 2;                     /* ensures each unordered pair counted once */
    for (idx_t x = 1; x <= limit; ++x) {
        idx_t y = n - x;
        if (!squarefree_flag[x] || !squarefree_flag[y]) continue;
        if (prime_flag[x] || prime_flag[y]) continue;
        ++count;
    }
    return count;
}
