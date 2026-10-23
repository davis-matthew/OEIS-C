/*
 * A166468  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -3*a(n-12) + 2*Sum_{k=1..11} a(n-k) (n >= 13) with
 *            initial values: a(0)=1, a(1)=4, a(2)=12, a(3)=36, a(4)=108,
 *            a(5)=324, a(6)=972, a(7)=2916, a(8)=8748, a(9)=26244,
 *            a(10)=78732, a(11)=236196, a(12)=708582
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _base[13] = {
    (seq_t)1, (seq_t)4, (seq_t)12, (seq_t)36, (seq_t)108,
    (seq_t)324, (seq_t)972, (seq_t)2916, (seq_t)8748,
    (seq_t)26244, (seq_t)78732, (seq_t)236196, (seq_t)708582
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;       /* current allocation size   */
    static idx_t computed = (idx_t)-1; /* highest index already computed */

    /* Ensure the cache holds indices 0..n and at least the first 13 terms. */
    size_t needed = (size_t)(n + 1);
    if (needed < 13) needed = 13;          /* always keep space for the base */
    if (cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);     /* allocation failed */
        cache = tmp;
        cap   = needed;

        /* Initialise base values once. */
        if (computed == (idx_t)-1) {
            for (idx_t i = 0; i < 13; ++i) cache[i] = _base[i];
            computed = 12;
        }
    }

    /* Return immediately if already known. */
    if (n <= computed) return cache[n];

    /* Compute values from computed+1 up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 11; ++k) sum += cache[i - k];   /* previous 11 terms */
        sum = sum + sum;                                        /* 2 * sum */

        /* 3 * a(i-12) via two additions to avoid overflow in small types */
        seq_t tri = cache[i - 12] + cache[i - 12];
        tri += cache[i - 12];

        cache[i] = sum - tri;  /* a(i) = 2*sum - 3*a(i-12) */
    }
    computed = n;
    return cache[n];
}
