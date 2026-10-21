/*
 * A073892  a(1) = 1, a(2n) = a(2n-1) + p(n) and a(2n+1) = a(2n) - c(n), where
 *          p(n)=A000040(n) and c(n)=A002808(n) are the n-th composite and
 *          n-th prime numbers, respectively.
 * https://oeis.org/A073892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 29, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A073892:  a(1)=1;  
   a(2n)=a(2n‑1)+p(n)             with p(n)=n‑th prime  (A000040)  
   a(2n+1)=a(2n)-c(n)             with c(n)=n‑th composite (A002808)  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Global prime / composite tables (static, lazily built)            */
/* ------------------------------------------------------------------ */
static idx_t *primes       = NULL; /* 1‑based: primes[0] is the 1st prime */
static idx_t primes_cnt    = 0;
static idx_t *composites   = NULL; /* 1‑based: composites[0] is the 1st composite */
static idx_t composites_cnt= 0;
static idx_t sieve_limit   = 0;    /* last limit we sieved to */

/* ------------------------------------------------------------------ */
/* Build / extend prime / composite tables up to 'need' entries       */
/* ------------------------------------------------------------------ */
static void extend_primes_composites(idx_t need)
{
    if (need <= primes_cnt && need <= composites_cnt)
        return;   /* already have enough */

    idx_t limit = sieve_limit ? sieve_limit : 1000;

    while (1) {
        /* Sieve up to 'limit' */
        bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
        for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (idx_t i = 2; i * i <= limit; ++i)
            if (is_prime[i])
                for (idx_t j = i * i; j <= limit; j += i)
                    is_prime[j] = false;

        /* Count how many primes and composites we have */
        idx_t p_est = 0, c_est = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (is_prime[i]) ++p_est;
        for (idx_t i = 4; i <= limit; ++i)
            if (!is_prime[i]) ++c_est;

        if (p_est >= need && c_est >= need) {
            /* Allocate new tables */
            free(primes);
            free(composites);
            primes      = (idx_t*)malloc(need * sizeof(idx_t));
            composites  = (idx_t*)malloc(need * sizeof(idx_t));
            primes_cnt  = 0;
            composites_cnt = 0;

            for (idx_t i = 2; i <= limit && primes_cnt < need; ++i)
                if (is_prime[i]) primes[primes_cnt++] = i;

            for (idx_t i = 4; i <= limit && composites_cnt < need; ++i)
                if (!is_prime[i]) composites[composites_cnt++] = i;

            sieve_limit = limit;
            free(is_prime);
            break;
        } else {
            free(is_prime);
            limit <<= 1;   /* double the limit and retry */
        }
    }
}

/* ------------------------------------------------------------------ */
/* Sequence cache (static, lazily computed)                          */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache  = NULL;  /* 1‑based access, seq_cache[0] unused */
static idx_t seq_cap     = 0;
static idx_t seq_last    = 0;     /* highest computed index */

/* ------------------------------------------------------------------ */
/* Extend the cache to cover at least index 'n'                       */
/* ------------------------------------------------------------------ */
static void extend_sequence(idx_t n)
{
    if (n <= seq_last)
        return;   /* already computed */

    /* Grow cache array if necessary */
    size_t new_cap = seq_cap ? seq_cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    if (new_cap > seq_cap) {
        seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap   = new_cap;
    }

    /* Ensure we have enough primes / composites for indices up to n/2 */
    idx_t need = (n + 1) / 2;
    extend_primes_composites(need);

    /* Compute missing terms */
    for (idx_t i = seq_last + 1; i <= n; ++i) {
        if (i == 1) {
            seq_cache[1] = (seq_t)1;
        } else if ((i & 1) == 0) {                /* even */
            idx_t k = i >> 1;                     /* k-th prime needed */
            seq_cache[i] = seq_cache[i - 1] + (seq_t)primes[k - 1];
        } else {                                 /* odd >1 */
            idx_t k = (i - 1) >> 1;               /* k-th composite needed */
            seq_cache[i] = seq_cache[i - 1] - (seq_t)composites[k - 1];
        }
    }
    seq_last = n;
}

/* ------------------------------------------------------------------ */
/* Public function required by the harness                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined, but return 0 */
        return (seq_t)0;
    extend_sequence(n);
    return seq_cache[n];
}
