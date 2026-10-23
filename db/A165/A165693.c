/*
 * A165693  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A165693
   a(0)=1, a(1)=42, a(2)=1722, a(3)=70602, a(4)=2894682,
   a(5)=118681962, a(6)=4865960442, a(7)=199504378122,
   a(8)=8179679503002.
   a(9)   = 40*a(8)+…+40*a(1) – 820*a(0) + 1
   For n ≥10:
   a(n) = 40*a(n-1)+…+40*a(n-8) – 820*a(n-9). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;   /* sequence values */
    static size_t  cap    = 0;      /* current capacity   */
    static bool    init   = false;  /* cache initialised? */
    static idx_t   last   = 0;      /* highest index computed */

    /* Initial values a(0) … a(8) */
    static const seq_t init_vals[9] = {
        (seq_t)1ULL,
        (seq_t)42ULL,
        (seq_t)1722ULL,
        (seq_t)70602ULL,
        (seq_t)2894682ULL,
        (seq_t)118681962ULL,
        (seq_t)4865960442ULL,
        (seq_t)199504378122ULL,
        (seq_t)8179679503002ULL
    };

    /* One‑time allocation and initialisation */
    if (!init) {
        cap    = 9;                      /* need at least 9 elements */
        cache  = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failed */
        for (idx_t i = 0; i < 9; ++i) cache[i] = init_vals[i];
        last   = 8;
        init   = true;
    }

    /* Grow capacity if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* reallocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= last) return cache[n];

    /* Compute terms up to n using the linear recurrence */
    for (idx_t k = last + 1; k <= n; ++k) {
        /* 40*a(k-1) … 40*a(k-8) */
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= 8; ++j) sum += cache[k - j] * (seq_t)40;
        /* Subtract 820*a(k-9) */
        sum -= cache[k - 9] * (seq_t)820;
        /* Add the numerator term for k == 9 */
        if (k == 9) sum += (seq_t)1;
        cache[k] = sum;
    }
    last = n;
    return cache[n];
}
