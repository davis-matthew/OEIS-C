/*
 * A107928  a(n) is the numerator of harmonic mean of a(n-1) and a(n-2).
 * https://oeis.org/A107928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=2, a(2)=3, n>=3: a(n) = numerator(
 *            2*a(n-1)*a(n-2)/(a(n-2)+a(n-1)) )
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t my_gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* n must be >= 1 */

    /* base terms */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;

    /* static memoisation table */
    static seq_t *terms = NULL;          /* term values, indices start at 1 */
    static idx_t cap   = 0;              /* current allocation size */
    static idx_t high  = 0;              /* highest n already computed */

    /* initial allocation */
    if (!terms) {
        cap = 4;                         /* at least indices 0..3 */
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms) return 0;
        for (size_t i = 0; i < cap; ++i) terms[i] = 0;
        terms[1] = (seq_t)2;
        terms[2] = (seq_t)3;
        high    = 2;
    }

    /* grow storage if required */
    if (n >= cap) {
        size_t new_cap = cap * 2;
        if (new_cap <= (size_t)n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = 0;
        terms = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = high + 1; i <= n; ++i) {
        seq_t x = terms[i - 1];
        seq_t y = terms[i - 2];

        /* use 128‑bit intermediate to avoid overflow */
        unsigned __int128 num128 = (unsigned __int128)2 * (unsigned __int128)x * (unsigned __int128)y;
        seq_t den = x + y;                /* fits in seq_t */

        seq_t rem = (seq_t)(num128 % den);   /* remainder fits in seq_t */
        seq_t g   = my_gcd(den, rem);       /* gcd fits in seq_t */

        seq_t val = (seq_t)(num128 / g);     /* division still fits seq_t */

        terms[i] = val;
    }
    high = n;

    return terms[n];
}
