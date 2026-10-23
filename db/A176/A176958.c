/*
 * A176958  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=5, k=-1 and l=-1.
 * https://oeis.org/A176958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 5 * a(n+1) = ∑_{p=0..n} a(p)*a(n-p) + k*(n+1) +
 *            l * with k = -1, l = -1 * * Thus for n ≥ 1 * a(n+1) = ∑_{p=0..n}
 *            a(p)*a(n-p) – (n+1) – 1 * = ∑_{p=0..n} a(p)*a(n-p) – n – 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* This function may be called in arbitrary order; it memoises
 * computed terms in a static dynamic array. */
seq_t a(idx_t n)
{
    /* Cached sequence values */
    static seq_t *terms = NULL;
    static size_t len  = 0;   /* number of values currently stored (terms[0..len-1]) */
    static size_t cap  = 0;   /* size of allocated array */

    /* Ensure capacity up to n */
    if ((size_t)n >= cap) {
        size_t new_cap = (cap ? cap * 2 : 16);
        while (new_cap <= (size_t)n) new_cap <<= 1;
        if (terms == NULL) {
            terms = (seq_t*)malloc(new_cap * sizeof(seq_t));
        } else {
            void *tmp = realloc(terms, new_cap * sizeof(seq_t));
            terms = (seq_t*)tmp;
        }
        cap = new_cap;
    }

    /* Initialise first two terms if necessary */
    if (len == 0) {
        terms[0] = (seq_t)1;
        if (cap > 1) terms[1] = (seq_t)5;   /* cap >= 2 after allocation above */
        len = (cap >= 2 ? 2 : 1);
    }

    /* Return cached value if already available */
    if ((size_t)n < len)
        return terms[n];

    /* Compute missing terms */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        /* i corresponds to index of a(i). For i >= 2, n = i-1 in recurrence. */
        seq_t conv = 0;
        /* convolution sum: p = 0 .. i-1 of a[p] * a[(i-1)-p] */
        for (idx_t p = 0; p < i; ++p)
            conv += terms[p] * terms[(size_t)(i - 1) - p];
        /* Subtract (i + 1) because k*(i-1)+l = -(i-1)-1 = -(i+1) */
        terms[i] = conv - ((seq_t)i + 1);
    }
    len = (size_t)n + 1;
    return terms[n];
}
