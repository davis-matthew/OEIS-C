/*
 * A167893  a(n) = Sum_{k=1..n} Catalan(k)^3.
 * https://oeis.org/A167893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} Catalan(k)^3 (n is 1‑based, a(1) = 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute the sum of Cubes of Catalan numbers up to n */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *sum = NULL;         /* array of partial sums a(i) */
    static size_t capacity = 0;       /* allocated size of sum[]    */
    static idx_t  curr   = 0;        /* highest index already computed */
    static seq_t  last_cat = 0;      /* Catalan(curr)        */

    /* --- first time: initialise storage and first values --- */
    if (curr == 0) {
        capacity = 2;                         /* indices 0 and 1 */
        sum = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!sum) return 0;                   /* out of memory */

        sum[0] = 0;                           /* not used but convenient */
        sum[1] = 1;                           /* a(1) = Catalan(1)^3 = 1 */
        last_cat = 1;                         /* Catalan(1)          */
        curr = 1;
    }

    /* --- grow storage if we need to address larger indices --- */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity;
        while ((size_t)n + 1 > new_cap)
            new_cap = new_cap ? new_cap * 2 : 1;

        seq_t *tmp = (seq_t *)realloc(sum, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* out of memory */
        sum = tmp;
        capacity = new_cap;
    }

    /* --- return already computed value --- */
    if (n <= curr) return sum[n];

    /* --- extend computation to the requested n --- */
    for (idx_t k = curr + 1; k <= n; ++k) {
        /* Catalan(k) = Catalan(k-1) * 2*(2k-1)/(k+1) */
        seq_t temp = last_cat * 2u;          /* 2 * Catalan(k-1)  */
        temp = temp * (seq_t)(2 * k - 1);    /* 2*(2k-1)*Catalan(k-1) */
        seq_t cat = temp / (seq_t)(k + 1);   /* divide exactly */

        /* cube = cat^3 */
        seq_t cube = cat * cat;
        cube = cube * cat;

        sum[k] = sum[k - 1] + cube;          /* partial sum */
        last_cat = cat;
        curr = k;
    }

    return sum[n];
}
