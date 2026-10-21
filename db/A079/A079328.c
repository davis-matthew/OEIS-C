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
 * Helpers:   build_twin_foll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A079328 – average of two consecutive smaller twin‑prime pair members
    Formula: a(n) = (f(n) + f(n+1)) / 2
    where f(n) = the smaller member of the n‑th pair of twin primes. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_twin_foll.h"

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
        free(twin_f);
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
