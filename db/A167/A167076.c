/*
 * A167076  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 21*(a(n-1)+…+a(n-12)) – 231*a(n-13),
   with a(0)=1, a(1)=23, a(2)=506, a(3)=11132, a(4)=244904,
   a(5)=5387888, a(6)=118533536, a(7)=2607737792,
   a(8)=57370231424, a(9)=1262145091328, a(10)=27767192009216,
   a(11)=610878224202752, a(12)=13439320932460544.
   For n = 13 the same formula is used with an additional +1
   (a(13) = 21*Σ_{k=1}^{12}a(13-k) – 231*a0 + 1). */

#include <stdlib.h>  /* malloc, free, abort */

seq_t a(idx_t n)
{
    /* Cache of computed terms (dynamic array). */
    static seq_t *cache   = NULL;   /* storage */
    static size_t size    = 0;      /* capacity of the array */
    static idx_t  count   = 0;      /* number of terms actually stored */

    /* Initial values up to a(12). */
    static const seq_t init_vals[13] = {
        (seq_t)1ULL,
        (seq_t)23ULL,
        (seq_t)506ULL,
        (seq_t)11132ULL,
        (seq_t)244904ULL,
        (seq_t)5387888ULL,
        (seq_t)118533536ULL,
        (seq_t)2607737792ULL,
        (seq_t)57370231424ULL,
        (seq_t)1262145091328ULL,
        (seq_t)27767192009216ULL,
        (seq_t)610878224202752ULL,
        (seq_t)13439320932460544ULL
    };

    /* Initialise the cache on first call. */
    if (!cache) {
        size  = 13;               /* at least as many as the initial values */
        cache = (seq_t *)malloc(sizeof(seq_t) * size);
        if (!cache) abort();
        for (idx_t i = 0; i < 13; ++i)
            cache[i] = init_vals[i];
        count = 13;                /* exactly 13 values stored */
    }

    /* Grow capacity if needed. */
    if ((size_t)n + 1 > size) {
        size_t new_size = size * 2;
        if (new_size <= (size_t)n + 1)
            new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (!tmp) abort();
        for (size_t i = 0; i < count; ++i)
            tmp[i] = cache[i];
        free(cache);
        cache = tmp;
        size  = new_size;
    }

    /* Return ready value if already known. */
    if (n < count)
        return cache[n];

    /* Compute missing values up to n (inclusive). */
    for (idx_t i = count; i <= n; ++i) {
        seq_t temp = 0;
        /* Sum of the previous 12 terms. */
        for (idx_t j = 1; j <= 12; ++j)
            temp += cache[i - j];

        seq_t val = temp * (seq_t)21;
        val -= cache[i - 13] * (seq_t)231;

        /* Special adjustment for n = 13. */
        if (i == 13)
            val += (seq_t)1;

        cache[i] = val;
        ++count;
    }

    return cache[n];
}
