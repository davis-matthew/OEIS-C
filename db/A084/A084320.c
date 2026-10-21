/*
 * A084320  Number of powers of two between 2 consecutive factorials (2!
 *          including).
 * https://oeis.org/A084320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log2((n+1)!)) - floor(log2(n!))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_words
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of powers of two between n! and (n+1)! (2! inclusive). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/bitlen_words.h"

/* Cached data -------------------------------------------------------------- */
static seq_t *log_cache = NULL;          /* floor(log2(k!)) values (index k) */
static idx_t log_cache_size = 0;         /* allocated size of log_cache */
static idx_t log_cache_max  = 0;         /* largest k for which log_cache[k] known */

static uint32_t *fact_words = NULL;      /* words of the running factorial */
static size_t fact_len = 0;              /* number of words used */
static size_t fact_alloc = 0;            /* allocated number of words */

/* Ensure that log_cache[0..n] and factorial up to n! are computed. */
static void ensure_cache(idx_t n)
{
    if (n <= log_cache_max)
        return;                          /* already computed */

    /*--- expand log_cache array ------------------------------------------ */
    if (log_cache == NULL) {
        log_cache_size = n + 1;
        log_cache = (seq_t *)calloc(log_cache_size, sizeof(seq_t));
        if (log_cache == NULL) abort();  /* allocation failure */
        log_cache[0] = (seq_t)0;         /* 0! = 1 → floor(log2(1)) = 0 */
    } else if (log_cache_size <= n) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(log_cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();
        log_cache = tmp;
        log_cache_size = new_size;
    }

    /*--- initialise factorial big integer if first use --------------------- */
    if (fact_words == NULL) {
        fact_alloc = 64;                 /* start with 64 words (~2048 bits) */
        fact_words = (uint32_t *)calloc(fact_alloc, sizeof(uint32_t));
        if (fact_words == NULL) abort();
        fact_len = 1;
        fact_words[0] = 1;               /* 0! = 1 */
    }

    /*--- compute factorial iteratively from current max up to n ------------ */
    for (idx_t k = log_cache_max + 1; k <= n; ++k) {
        uint32_t mul = (uint32_t)k;      /* k fits in 32 bits for all practical k */

        /* ensure space for potential carry word */
        if (fact_len == fact_alloc) {
            size_t new_alloc = fact_alloc * 2;
            uint32_t *tmp = (uint32_t *)realloc(fact_words, new_alloc * sizeof(uint32_t));
            if (tmp == NULL) abort();
            fact_words = tmp;
            fact_alloc = new_alloc;
        }

        uint64_t carry = 0;
        for (size_t i = 0; i < fact_len; ++i) {
            uint64_t prod = (uint64_t)fact_words[i] * mul + carry;
            fact_words[i] = (uint32_t)prod;
            carry = prod >> 32;
        }
        while (carry) {
            if (fact_len == fact_alloc) {
                size_t new_alloc = fact_alloc * 2;
                uint32_t *tmp = (uint32_t *)realloc(fact_words, new_alloc * sizeof(uint32_t));
                if (tmp == NULL) abort();
                fact_words = tmp;
                fact_alloc = new_alloc;
            }
            fact_words[fact_len++] = (uint32_t)carry;
            carry >>= 32;
        }

        /* store floor(log2(k!)) = bitlen(k!) – 1 */
        size_t len_bits = bitlen_words(fact_words, fact_len);
        log_cache[k] = (seq_t)(len_bits - 1);
    }

    log_cache_max = n;
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* index out of definition range */

    /* we need log_cache[n] and log_cache[n+1] */
    ensure_cache(n + 1);

    return log_cache[n + 1] - log_cache[n];
}
