/*
 * A128696  Alternating sum of the seventh powers of the first n Fibonacci
 *          numbers.
 * https://oeis.org/A128696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=1..n} (-1)^k * F(k)^7 (F(k) = Fibonacci number
 *            A000045(k)) Recurrence (from OEIS A128696): a(n) + 20 a(n‑1) -
 *            294 a(n‑2) - 819 a(n‑3) + 2912 a(n‑4) - 728 a(n‑5) - 1365 a(n‑6)
 *            + 252 a(n‑7) + 22 a(n‑8) - a(n‑9) = 0 Initialized for n = 0 … 8
 *            by direct summation
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 8, i64 n <= 14, i128 n <= 27,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/fib.h"

static seq_t *cache = NULL;      /* dynamic array of previously computed a(n) */
static idx_t cache_size = 0;     /* number of elements stored */

/* Compute a(0)..a(8) directly from the definition (used on first call) */
static void init_first_terms(void)
{
    const idx_t N = 9; /* cover indices 0 .. 8 */
    seq_t *new_cache = (seq_t *)malloc(N * sizeof(seq_t));
    if (!new_cache) abort();

    for (idx_t n = 0; n < N; ++n)
    {
        seq_t sum = 0;
        for (idx_t k = 1; k <= n; ++k)
        {
            seq_t fk = fib(k);          /* F(k) */
            seq_t f7 = fk;              /* fk^7 */
            for (int t = 1; t < 7; ++t) f7 *= fk;
            /* (-1)^k : negative when k is odd */
            if ((k & 1U) == 1U) f7 = -f7;
            sum += f7;
        }
        new_cache[n] = sum;
    }

    cache = new_cache;
    cache_size = N;
}

/* Extend cache so that it contains a(0)..a(n) */
static void extend_cache(idx_t n)
{
    if (cache_size == 0) init_first_terms();

    if (cache_size > n) return;   /* already have all needed terms */

    size_t needed = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache) abort();

    for (size_t i = cache_size; i < needed; ++i)
    {
        /* Use recurrence derived from the OEIS relation:
           a(i) = -20 a(i-1) + 294 a(i-2) + 819 a(i-3) - 2912 a(i-4)
                 + 728 a(i-5) + 1365 a(i-6) - 252 a(i-7) - 22 a(i-8) + a(i-9) */
        seq_t val = -((seq_t)20) * new_cache[i-1];
        val +=  ((seq_t)294) * new_cache[i-2];
        val +=  ((seq_t)819) * new_cache[i-3];
        val -=  ((seq_t)2912) * new_cache[i-4];
        val +=  ((seq_t)728) * new_cache[i-5];
        val +=  ((seq_t)1365) * new_cache[i-6];
        val -=  ((seq_t)252) * new_cache[i-7];
        val -=  ((seq_t)22)  * new_cache[i-8];
        val +=  new_cache[i-9];
        new_cache[i] = val;
    }

    cache = new_cache;
    cache_size = needed;
}

seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[(size_t)n];
}
