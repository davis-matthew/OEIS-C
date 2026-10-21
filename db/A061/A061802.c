/*
 * A061802  Sum of n-th row of triangle of primes: 2; 2 3 2; 2 3 5 3 2; 2 3 5
 *          7 5 3 2; ...; where n-th row contains 2n+1 terms.
 * https://oeis.org/A061802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(n)=a(n-1)+prime(n)+prime(n-1)
 * Range:     u8 n <= 9, u16 n <= 114, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime generation (trial division) ---------- */
static uint64_t *prime_buf   = NULL; /* 0‑based index: prime_buf[0] = 2 */
static size_t   prime_cap    = 0;
static size_t   prime_sz     = 0;    /* number of primes stored   */
static uint64_t next_candidate = 2;  /* next integer to test      */

static void ensure_primes(idx_t need)
{
    /* generate primes until index 'need' is present */
    while (prime_sz <= need) {
        uint64_t cand = next_candidate;
        bool is_prime = true;

        for (size_t i = 0; i < prime_sz && prime_buf[i]*prime_buf[i] <= cand; ++i) {
            if (cand % prime_buf[i] == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            if (prime_cap == prime_sz) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_buf = (uint64_t*)realloc(prime_buf, prime_cap * sizeof(uint64_t));
            }
            prime_buf[prime_sz++] = cand;
        }

        /* advance to next candidate (skip even numbers) */
        if (cand == 2) next_candidate = 3;
        else next_candidate += 2;
    }
}

/* ---------- sequence cache ---------- */
static seq_t *a_cache     = NULL;
static size_t a_cache_cap = 0;
static idx_t  a_cache_last = 0;  /* last valid index in the cache */

/* ---------- the sequence function ---------- */
seq_t a(idx_t n)
{
    /* allocate or grow the cache */
    if (a_cache == NULL) {
        a_cache_cap = n + 1;
        a_cache = (seq_t*)malloc(a_cache_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)2;      /* a(0) = 2 */
        a_cache_last = 0;
    } else if (a_cache_cap <= n) {
        a_cache_cap = n + 1;
        a_cache = (seq_t*)realloc(a_cache, a_cache_cap * sizeof(seq_t));
    }

    /* already computed? */
    if (n <= a_cache_last) return a_cache[n];

    /* need primes up to index n */
    ensure_primes(n);

    /* compute missing terms */
    seq_t val = a_cache[a_cache_last];
    for (idx_t i = a_cache_last + 1; i <= n; ++i) {
        val += (seq_t)prime_buf[i];       /* prime(n)   */
        val += (seq_t)prime_buf[i-1];     /* prime(n-1) */
        a_cache[i] = val;
    }
    a_cache_last = n;
    return val;
}
