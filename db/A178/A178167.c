/*
 * A178167  Binomial transform of odd primes.
 * https://oeis.org/A178167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{k=1..n} binomial(n-1, k-1) * p_k
 *   where p_k is the k-th odd prime (p_1 = 3, p_2 = 5, …).
 * This implements the binomial transform of the odd primes.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Prime generation of odd primes ---------- */
static idx_t *prime_arr = NULL;          /* array of odd primes */
static size_t prime_cnt = 0;             /* how many primes are stored           */
static size_t prime_cap = 0;             /* allocated capacity of prime_arr      */
static idx_t   prime_next = 3;           /* next odd candidate to test           */

/* Ensure that the first `need` odd primes are available in prime_arr. */
static void ensure_primes(idx_t need)
{
    if (prime_cnt >= need)
        return;

    /* grow prime array capacity if necessary */
    if (prime_cap < 64)
        prime_cap = 64;
    while (prime_cap < need)
        prime_cap <<= 1;

    /* allocate or reallocate prime array */
    if (prime_arr == NULL)
        prime_arr = (idx_t *)calloc(prime_cap, sizeof(idx_t));
    else
        prime_arr = (idx_t *)realloc(prime_arr, prime_cap * sizeof(idx_t));
    if (prime_arr == NULL)
        abort();  /* allocation failure */

    /* simple trial‑division generation of odd primes */
    while (prime_cnt < need) {
        idx_t cand = prime_next;
        prime_next += 2;  /* next odd candidate */

        bool is_prime = true;
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            idx_t p = prime_arr[pi];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            prime_arr[prime_cnt++] = cand;
    }
}

/* ---------- Sequence cache ---------- */
static seq_t *seq_cache = NULL;   /* array of computed a(n) values */
static size_t cache_cap = 0;      /* allocated size of seq_cache (>= n+1) */

/* ---------- OEIS sequence A178167 ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for OEIS, but safe */

    if ((size_t)n <= cache_cap)
        return seq_cache[n];

    /* Generate required primes */
    ensure_primes(n);

    /* Grow cache array to hold values up to n */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(seq_cache,
                                            new_cap * sizeof(seq_t));
        if (new_cache == NULL)
            abort();  /* allocation failure */
        seq_cache = new_cache;

        /* Compute new sequence values incrementally */
        for (idx_t i = (idx_t)cache_cap + 1; i <= n; ++i) {
            seq_t sum = 0;
            seq_t C   = 1;  /* binomial coefficient C(i-1,0) */
            for (idx_t k = 1; k <= i; ++k) {
                sum += C * (seq_t)prime_arr[k - 1];
                if (k < i)  /* prepare next coefficient C(i-1,k) */
                    C = C * (seq_t)(i - k) / (seq_t)k;
            }
            seq_cache[i] = sum;
        }
        cache_cap = n;
    }
    return seq_cache[n];
}
