/*
 * A165703  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165703
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

/* OEIS A165703: Number of reduced words of length n in the Coxeter group
   on 47 generators with relations (S_i)^2 = (S_i S_j)^9 = I.
   Recurrence (with D(t)=1-45 t-45 t^2-…-45 t^8+1035 t^9, N(t)=t^9+2t^8+…+2t+1):
   a(0)=1, a(1)=47, a(2)=2162, a(3)=99452, a(4)=4574792,
   a(5)=210440432, a(6)=9680259872, a(7)=445291954112,
   a(8)=20483429889152
   For n≥9:
     a(n) = 45*(a(n-1)+…+a(n-8)) – 1035*a(n-9) + c(n)
   where c(n)=1 if n==9 and c(n)=0 otherwise. */

#include <stdlib.h>

static seq_t *cache = NULL;       /* array of computed terms */
static size_t cache_cap = 0;      /* capacity of the array */
static size_t cache_len = 0;      /* number of terms currently valid in cache */

/* first nine values that fit in 64‑bit */
static const seq_t init_terms[9] = {
    (seq_t)1,
    (seq_t)47,
    (seq_t)2162,
    (seq_t)99452,
    (seq_t)4574792,
    (seq_t)210440432,
    (seq_t)9680259872ULL,
    (seq_t)445291954112ULL,
    (seq_t)20483429889152ULL
};

/* enlarge the cache so that index n is addressable */
static void expand_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_arr;
    cache_cap = new_cap;
}

/* initialise cache with the nine known terms */
static void init_cache(void)
{
    expand_cache(8);          /* ensures room for indices 0..8 */
    for (idx_t i = 0; i < 9; ++i)
        cache[i] = init_terms[i];
    cache_len = 9;            /* next free index is 9 */
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (cache == NULL)
        init_cache();

    /* value already known */
    if ((size_t)n < cache_len)
        return cache[n];

    /* make sure the array is large enough */
    expand_cache(n);

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t sum = 0;

        /* 45 * a(i‑1) … a(i‑8) */
        for (int k = 1; k <= 8; ++k)
            sum += (seq_t)45 * cache[i - k];

        /* subtract 1035 * a(i‑9) */
        sum -= (seq_t)1035 * cache[i - 9];

        /* add numerator coefficient for indices 0…9 */
        if (i == 9)          /* numerator coefficient of t^9 is 1 */
            sum += (seq_t)1;
        /* coefficients for 1…8 are 2, but i never in that range here */

        cache[i] = sum;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
