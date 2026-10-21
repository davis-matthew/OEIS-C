/*
 * A093572  Greatest prime factor of Product(k+prime(k): 1<=k<=n).
 * https://oeis.org/A093572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum prime factor of Π_{k=1..n} (k + prime(k))
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* External integer types supplied by the build */
typedef idx_t idx_t;   /* e.g. uint64_t */
typedef seq_t seq_t;   /* e.g. unsigned __int128 or arbitrary precision type */

/* --------------------------------------------------------------------- *
 *  Prime generation (sieve of Eratosthenes up to a fixed limit)          *
 * --------------------------------------------------------------------- */
static bool primes_ready = false;
static uint32_t *primes = NULL;   /* ascending list of primes */
static size_t primes_cnt  = 0;     /* number of primes stored */

/* Small fixed limit – enough to cover prime(10000) = 104729 */
#define PRIME_LIMIT 200000

static void init_primes(void)
{
    if (primes_ready) return;

    bool *is_comp = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_comp) return;                 /* allocation failure – leave empty */

    primes_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            /* store prime i */
            uint32_t *tmp = (uint32_t*)realloc(primes, (primes_cnt + 1) * sizeof(uint32_t));
            if (!tmp) { free(is_comp); return; }   /* allocation failure */
            primes = tmp;
            primes[primes_cnt++] = (uint32_t)i;

            /* mark composites */
            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }

    free(is_comp);
    primes_ready = true;
}

/* --------------------------------------------------------------------- *
 *  Auxiliary: largest prime factor of a seq_t value                    *
 * --------------------------------------------------------------------- */
static seq_t largest_prime_factor(seq_t t)
{
    seq_t max_p = 0;
    seq_t tmp   = t;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        seq_t psq  = (seq_t)p * (seq_t)p;
        if (psq > tmp) break;
        if (tmp % (seq_t)p == 0) {
            max_p = p;
            while (tmp % (seq_t)p == 0) tmp /= (seq_t)p;
        }
    }

    if (tmp > 1) max_p = tmp;   /* remaining prime factor */
    return max_p;
}

/* --------------------------------------------------------------------- *
 *  Memoization of a(n)                                                 *
 * --------------------------------------------------------------------- */
static seq_t *memo    = NULL;    /* memo[1..] holds computed values */
static size_t memo_cap = 0;       /* allocated capacity of memo */
static idx_t memo_max_n = 0;      /* largest n for which memo is filled */

/* Ensure memo can store indexes up to n inclusive */
static void ensure_memo_capacity(idx_t n)
{
    if ((size_t)n < memo_cap) return;
    size_t new_cap = memo_cap ? memo_cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *new_arr = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
    if (!new_arr) return;                 /* allocation failure – keep old */
    memset(new_arr + memo_cap, 0, (new_cap - memo_cap) * sizeof(seq_t));
    memo = new_arr;
    memo_cap = new_cap;
}

/* --------------------------------------------------------------------- *
 *  Main sequence function                                              *
 * --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* defined only for n≥1 */

    init_primes();                        /* make prime list available */

    ensure_memo_capacity(n);

    /* Default initialization for first access */
    if (n > 0 && memo[0] == 0) memo[0] = 0;

    /* Return cached value if already computed */
    if (n <= memo_max_n) return memo[n];

    /* Start from the last computed index */
    seq_t cum_max = memo[memo_max_n];

    /* Compute values for k = memo_max_n+1 .. n */
    for (idx_t k = memo_max_n + 1; k <= n; ++k) {
        /* kth prime (primes array is 0‑based) */
        if ((size_t)k > primes_cnt) return 0;   /* safety guard */
        uint32_t pk = primes[(size_t)k - 1];

        seq_t term = (seq_t)k + (seq_t)pk;
        seq_t local_max = largest_prime_factor(term);

        if (local_max > cum_max) cum_max = local_max;
        memo[(size_t)k] = cum_max;
    }

    memo_max_n = n;
    return memo[(size_t)n];
}
