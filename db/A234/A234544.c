/*
 * A234544  Number of (n+1) X (2+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) -30*a(n-2) -944*a(n-3) +5146*a(n-4)
 *            +8720*a(n-5) -112704*a(n-6) +148224*a(n-7) +392632*a(n-8)
 *            -632528*a(n-9) -548544*a(n-10) +867968*a(n-11) +341120*a(n-12)
 *            -478976*a(n-13) -76800*a(n-14) +92160*a(n-15), for n > 15;
 *            initial values for n = 1 … 15 are listed below
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Initial 15 terms of the sequence */
    static const seq_t init[15] = {
        1212ULL,          /* a(1) */
        6896ULL,          /* a(2) */
        39188ULL,         /* a(3) */
        222988ULL,        /* a(4) */
        1269812ULL,       /* a(5) */
        7237508ULL,       /* a(6) */
        41285540ULL,      /* a(7) */
        235717236ULL,     /* a(8) */
        1346930052ULL,    /* a(9) */
        7703391252ULL,    /* a(10) */
        44093945060ULL,   /* a(11) */
        252614282612ULL,  /* a(12) */
        1448426221700ULL, /* a(13) */
        8312186475348ULL, /* a(14) */
        47741148641380ULL /* a(15) */
    };

    static seq_t *seq = NULL;          /* array holding computed terms  */
    static size_t capacity = 0;        /* allocated size of the array      */
    static size_t computed = 0;        /* largest n already computed      */

    if (!seq) { /* initialise the storage with the first 15 values */
        capacity = 15;
        seq = (seq_t*)malloc(capacity * sizeof(seq_t));
        for (size_t i = 0; i < 15; ++i) seq[i] = init[i];
        computed = 15;
    }

    /* Expand storage if necessary */
    if ((size_t)n > capacity) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed – return zero */
        seq = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n using the 15‑term recurrence */
    if ((size_t)n > computed) {
        for (size_t m = computed + 1; m <= (size_t)n; ++m) {
            seq_t pos = (seq_t)0;
            seq_t neg = (seq_t)0;

            /* Positive coefficient terms */
            pos += seq[m - 2] * (seq_t)17;
            pos += seq[m - 5] * (seq_t)5146;
            pos += seq[m - 6] * (seq_t)8720;
            pos += seq[m - 8] * (seq_t)148224;
            pos += seq[m - 9] * (seq_t)392632;
            pos += seq[m -12] * (seq_t)867968;
            pos += seq[m -13] * (seq_t)341120;
            pos += seq[m -16] * (seq_t)92160;

            /* Negative coefficient terms */
            neg += seq[m - 3] * (seq_t)30;
            neg += seq[m - 4] * (seq_t)944;
            neg += seq[m - 7] * (seq_t)112704;
            neg += seq[m -10] * (seq_t)632528;
            neg += seq[m -11] * (seq_t)548544;
            neg += seq[m -14] * (seq_t)478976;
            neg += seq[m -15] * (seq_t)76800;

            seq[m - 1] = pos - neg;
        }
        computed = (size_t)n;
    }

    return seq[(size_t)n - 1];
}
