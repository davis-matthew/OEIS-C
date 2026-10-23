/*
 * A172383  a(0)=1, otherwise a(n) = Sum_{k=0..floor((n-1)/2)}
 *          binomial(n-k-1,k)*a(n-1-2*k).
 * https://oeis.org/A172383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; for n≥1, a(n) = Σ_{k=0..⌊(n-1)/2⌋} C(n‑k‑1, k) *
 *            a(n‑1‑2k) (OEIS A172383)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 37, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Multiplicative binomial coefficient C(a,b) for arbitrary‑precision seq_t. */
static seq_t
binom_calc(idx_t a, idx_t b)
{
    if (b > a) return (seq_t)0;
    if (b == 0 || b == a) return (seq_t)1;
    if (b > a - b) b = a - b;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= b; ++i) {
        seq_t term = (seq_t)(a - b + i);
        res = (res * term) / (seq_t)i;
    }
    return res;
}

/* Sequence A172383 with memoisation. */
seq_t
a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_last = 0;   /* last index that has been filled */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Initialise cache with a(0)=1 on first use. */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache_cap = 1;
        cache_last = 0;
    }

    /* Ensure capacity for index n. */
    if ((idx_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;           /* realloc failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n if necessary. */
    if (n > cache_last) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            idx_t maxk = (i - 1) / 2;
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= maxk; ++k) {
                seq_t coeff = binom_calc(i - k - 1, k);
                seq_t prod  = cache[i - 1 - 2 * k] * coeff;
                sum += prod;
            }
            cache[i] = sum;
        }
        cache_last = n;
    }

    return cache[n];
}
