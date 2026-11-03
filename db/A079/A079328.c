/*
 * A079328  Let f(n)=A001359(n) be the smaller member of the n-th pair of twin
 *          primes. Then a(n) is the average of f(n) and f(n+1).
 * https://oeis.org/A079328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A079328 – average of two consecutive smaller twin‑prime pair members
    Formula: a(n) = (f(n) + f(n+1)) / 2
    where f(n) = the smaller member of the n‑th pair of twin primes. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* ------------------------------------------------------------------ */
static void
build_twin_foll(uint64_t limit, seq_t **out_arr, size_t *out_count)
{
    /* sieve up to limit (inclusive) */
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!is_prime) {
        exit(1);
    }
    for (uint64_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count twin pairs */
    size_t cnt = 0;
    for (uint64_t i = 3; i + 2 <= limit; ++i) {
        if (is_prime[i] && is_prime[i + 2])
            ++cnt;
    }

    /* Allocate result array */
    seq_t *arr = mem_new(seq_t, cnt);
    if (!arr) {
        free(is_prime);
        exit(1);
    }

    /* Store twin primes */
    size_t idx = 0;
    for (uint64_t i = 3; i + 2 <= limit; ++i) {
        if (is_prime[i] && is_prime[i + 2])
            arr[idx++] = (seq_t)i;
    }

    free(is_prime);
    *out_arr    = arr;
    *out_count  = cnt;
}

/* ------------------------------------------------------------------ */
/*  Helper: build the first `cnt` members of the twin‑prime sequence  */

/* ------------------------------------------------------------------ */
/*  Lazy static cache for f(n)                                            */
/* ------------------------------------------------------------------ */
static seq_t *twin_f          = NULL;   /* array of f(1), f(2), … */
static size_t twin_f_count    = 0;       /* number of elements in twin_f */
static uint64_t sieve_limit   = 2000000ULL;   /* initial limit */
static int initialized        = 0;

/* Ensure the cache holds at least `need` elements (0‑based). */
static void
ensure_twin_f(size_t need)
{
    if (!initialized) {
        build_twin_foll(sieve_limit, &twin_f, &twin_f_count);
        initialized = 1;
    }
    /* Grow until enough elements are available. */
    while (twin_f_count < need) {
        sieve_limit <<= 1;                       /* double the limit */
        mem_free(twin_f);
        build_twin_foll(sieve_limit, &twin_f, &twin_f_count);
    }
}

/* ------------------------------------------------------------------ */
/*  Main function: returns a(n) for n ≥ 1                                */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                  /* offset clipping – not used in tests */
    /* Ensure we have f(n) and f(n+1) → need n+1 elements (0‑based). */
    ensure_twin_f((size_t)(n + 1));

    /* f(n) is twin_f[n-1], f(n+1) is twin_f[n] */
    seq_t f_n  = twin_f[n - 1];
    seq_t f_n1 = twin_f[n];
    return (f_n + f_n1) / 2;
}
