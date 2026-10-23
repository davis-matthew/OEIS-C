/*
 * A170383  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For 1 ≤ n ≤ 42: a(n) = 36 * (a(n-1) + … + a(n-n)) + 2.
 *            a(43) = 36 * (a(42)+…+a(0)) - 666 * a(0) + 1; For n ≥ 44: a(n) =
 *            36 * (a(n-1)+ … + a(n-42)) - 666 * a(n-43). (Thus for n > 43 the
 *            RHS constant term is 0.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_reserve = 0;   /* number of allocated slots */
static idx_t cache_size = 0;     /* number of computed entries */

/* Multiplication helpers – use 128‑bit intermediate when seq_t is 64‑bit
   to avoid overflow in the 64‑bit build. */
#if defined(__SIZEOF_INT128__)
#define MUL36(x)  ((sizeof(seq_t)==8) ? (seq_t)((__int128)36 * (x)) : (36 * (seq_t)(x)))
#define MUL666(x) ((sizeof(seq_t)==8) ? (seq_t)((__int128)666 * (x)) : (666 * (seq_t)(x)))
#else
#define MUL36(x)  (36 * (seq_t)(x))
#define MUL666(x) (666 * (seq_t)(x))
#endif

/* Grow the cached array to at least n+1 elements.  Cached values are kept. */
static void extend_cache(idx_t n)
{
    if (cache == NULL) {
        cache_reserve = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_reserve * sizeof(seq_t));
        cache_size = 0;
    } else if ((size_t)(n + 1) > cache_reserve) {
        cache_reserve = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, cache_reserve * sizeof(seq_t));
    }

    for (idx_t k = cache_size; k <= n; ++k) {
        seq_t sum = 0;
        /* 36 * sum of the last up to 42 previous entries */
        for (idx_t i = 1; i <= 42; ++i) {
            if (k < i) break;
            sum += MUL36(cache[k - i]);
        }
        if (k >= 43)
            sum -= MUL666(cache[k - 43]);

        /* add the RHS constant term
           (value 1 for n=0, 2 for 1≤n≤42, 1 for n=43, 0 otherwise) */
        if (k == 0)
            sum += 1;
        else if (k <= 42)
            sum += 2;
        else if (k == 43)
            sum += 1;

        cache[k] = sum;
    }
    cache_size = n + 1;
}

/* Public interface – returns a(n) for any n ≥ 0, in any order of calls. */
seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[n];
}
