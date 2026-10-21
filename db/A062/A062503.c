/*
 * A062503  Squarefree numbers squared.
 * https://oeis.org/A062503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (n-th squarefree integer)^2 a(n) = A000290(A005117(n))
 * Range:     u8 n <= 11, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- Cache of squarefree numbers ---------- */
static seq_t *sqfs = NULL;      /* array of computed squarefree integers */
static size_t sqfs_len = 0;     /* number of stored squarefree integers */
static size_t sqfs_cap = 0;     /* allocated capacity of sqfs */
static size_t sqfs_limit = 0;   /* largest integer that has been inspected */

/* allocate memory with correct cast for C and C++ */
static void *safe_malloc(size_t size) {
#ifdef __cplusplus
    return malloc(size);
#else
    return malloc(size);
#endif
}
static void *safe_realloc(void *ptr, size_t size) {
#ifdef __cplusplus
    return realloc(ptr, size);
#else
    return realloc(ptr, size);
#endif
}

/* Build all squarefree numbers up to 'limit' (inclusive) */
static void compute_to_limit(size_t limit) {
    if (limit <= sqfs_limit) return;   /* already done */

    unsigned char *ok      = (unsigned char *)safe_malloc(limit + 1);
    unsigned char *is_prime = (unsigned char *)safe_malloc(limit + 1);
    if (!ok || !is_prime) { exit(1); }

    for (size_t i = 0; i <= limit; ++i) {
        ok[i]      = 1;
        is_prime[i] = 1;
    }
    is_prime[0] = is_prime[1] = 0;

    /* Sieve to find primes */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Mark multiples of prime squares as not squarefree */
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            size_t p2 = i * i;
            for (size_t j = p2; j <= limit; j += p2) {
                ok[j] = 0;
            }
        }
    }

    /* Ensure storage for all numbers up to 'limit' */
    if (sqfs_cap < limit + 1) {
        size_t new_cap = limit + 1;
        seq_t *tmp = (seq_t *)safe_realloc(sqfs, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        sqfs = tmp;
        sqfs_cap = new_cap;
    }

    /* Store squarefree numbers into cache */
    sqfs_len = 0;
    for (size_t i = 1; i <= limit; ++i) {
        if (ok[i]) {
            sqfs[sqfs_len++] = (seq_t)i;
        }
    }

    free(ok);
    free(is_prime);
    sqfs_limit = limit;
}

/* Ensure that the cache contains at least 'n' squarefree numbers. */
static void ensure_n(idx_t n) {
    if (sqfs_len >= (size_t)n) return;
    size_t lim = (sqfs_limit == 0) ? 16 : sqfs_limit;
    while (sqfs_len < (size_t)n) {
        compute_to_limit(lim);
        if (sqfs_len >= (size_t)n) break;
        lim *= 2;          /* double inspected range if needed */
    }
}

/* Sequence generator: returns the n-th term of A062503. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* sequence starts at offset 1 */
    ensure_n(n);
    seq_t base = sqfs[(size_t)(n - 1)];   /* n-th squarefree integer */
    return base * base;                   /* its square */
}
