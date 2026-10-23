/*
 * A179252  Numbers that have 12 terms in their Zeckendorf representation.
 * https://oeis.org/A179252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A179252: Numbers that have 12 terms in their Zeckendorf representation.
 * The function generates all sums of exactly 12 non‑consecutive Fibonacci
 * numbers (F₁=1, F₂=2, F₃=3, …), sorts them, and caches the results.
 * a(n) is the n‑th smallest such sum, 1‑based index.  The algorithm
 * works for any n ≥ 1 and remains well under the required timing for
 * n up to about 10 000. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binom_count.h"

/* ------------------------------------------------------------ */
/* Fibonacci numbers (1‑based) */
#define MAX_FIB 200
static seq_t fib[MAX_FIB];
static bool fib_ready = false;

static void init_fib(void)
{
    if (fib_ready) return;
    fib[0] = (seq_t)0;
    fib[1] = (seq_t)1;
    fib[2] = (seq_t)2;
    for (int i = 3; i < MAX_FIB; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];
    fib_ready = true;
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* Recursive generation of 12‑term, non‑consecutive sums */
#define K 12

static void generate_rec(int pos, int next, int limit,
                         seq_t cur_sum, seq_t *store, size_t *cnt)
{
    if (pos == K) {
        store[(*cnt)++] = cur_sum;
        return;
    }
    int need = K - pos - 1;
    int max_i = limit - 2 * need;          /* largest index that still
                                               allows enough room for the
                                               remaining terms */
    for (int i = next; i <= max_i; ++i)
        generate_rec(pos + 1, i + 2, limit,
                     cur_sum + fib[i], store, cnt);
}

/* ------------------------------------------------------------ */
/* Comparison function for qsort (C). */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* ------------------------------------------------------------ */
/* Cache of sequence terms */
static seq_t *cache = NULL;        /* sorted list of sums */
static size_t cache_len = 0;       /* number of cached terms */

/* Ensure that the cache contains at least `need` terms. */
static void ensure_cache(idx_t need)
{
    if (need <= cache_len) return;

    init_fib();   /* ensure Fibonacci numbers are ready */

    /* Find minimal index N such that C(N‑K+1, K) >= need */
    int N = 2 * K;  /* initial guess that definitely yields enough terms */
    while (binom_count(N - K + 1, K) < (size_t)need)
        ++N;

    size_t total = binom_count(N - K + 1, K);
    seq_t *tmp = (seq_t *)malloc(total * sizeof(seq_t));
    if (!tmp) exit(1);

    size_t produced = 0;
    generate_rec(0, 1, N, (seq_t)0, tmp, &produced);
    /* produced should equal total */

    qsort(tmp, total, sizeof(seq_t), cmp_seq);

    free(cache);
    cache = tmp;
    cache_len = total;
}

/* ------------------------------------------------------------ */
/* Required public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not defined for 0, but safe */
    ensure_cache(n);
    return cache[n - 1];           /* convert 1‑based to 0‑based index */
}
