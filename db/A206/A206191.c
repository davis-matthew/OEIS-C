/*
 * A206191  Number of (n+1) X 3 0..3 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to two.
 * https://oeis.org/A206191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A206191
 * Empirical recurrence:
 *   a(n) = 23*a(n-1) -97*a(n-2) +169*a(n-3) -151*a(n-4) +250*a(n-5)
 *          -178*a(n-6) +66*a(n-7) -64*a(n-8) +8*a(n-9) +4*a(n-10)
 * Initial values: a(1)..a(10) as provided in the OEIS entry
 */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* offset 1 – undefined for n==0, just return 0 */
    if (n == 0) return 0;

    /* initial terms a(1)..a(10) */
    static const seq_t init[10] = {
        (seq_t)1012ULL,
        (seq_t)18172ULL,
        (seq_t)329216ULL,
        (seq_t)5972392ULL,
        (seq_t)108363344ULL,
        (seq_t)1966171084ULL,
        (seq_t)35674679732ULL,
        (seq_t)647289744976ULL,
        (seq_t)11744575881312ULL,
        (seq_t)213096317372352ULL
    };

    /* return immediately if we are still in the initial range */
    if (n <= 10) return init[n - 1];

    /* coefficients of the recurrence (k = 0..9 corresponds to a(n-1)..a(n-10)) */
    static const int coeff[10] = {
        23,   /*  a(n-1)   */
       -97,   /*  a(n-2)   */
       169,   /*  a(n-3)   */
      -151,   /*  a(n-4)   */
       250,   /*  a(n-5)   */
      -178,   /*  a(n-6)   */
        66,   /*  a(n-7)   */
      -64,   /*  a(n-8)   */
        8,    /*  a(n-9)   */
        4     /*  a(n-10)  */
    };

    /* dynamic cache for computed terms */
    static seq_t *cache = NULL;    /* array of computed a(i) values, 0‑based */
    static size_t size   = 0;      /* number of values actually stored */
    static size_t cap    = 0;      /* allocated capacity of the array */

    /* lazily initialise the cache with the first ten terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(10 * sizeof(seq_t));
        if (!cache) return 0;                  /* allocation failure (unlikely) */
        for (size_t i = 0; i < 10; ++i) cache[i] = init[i];
        size = 10;
        cap  = 10;
    }

    /* extend capacity if needed */
    if (cap < (size_t)n) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 10;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute terms up to index n-1 (zero‑based) if not already done */
    for (idx_t i = size; i < n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int k = 0; k < 10; ++k) {
            idx_t idx = i - (idx_t)k - 1;          /* index of a(i+1-k) in cache */
            int   c   = coeff[k];
            seq_t term = cache[idx] * (seq_t)(c >= 0 ? c : -c);
            if (c >= 0)
                sum_pos += term;
            else
                sum_neg += term;
        }
        cache[i] = sum_pos - sum_neg;               /* result is guaranteed non‑negative */
        ++size;                                      /* one more term computed */
    }

    return cache[n - 1];
}
