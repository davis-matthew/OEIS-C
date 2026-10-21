/*
 * A056591  Fifth column sequence of triangle A056588.
 * https://oeis.org/A056591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A056591.
   Recurrence of order 15 derived from the denominator of the generating function.
   Sequence starts with the known first 15 terms. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* --- Static state ----------------------------------------------- */
    static seq_t *table = NULL;         /* computed values */
    static size_t table_size = 0;       /* number of computed values */
    static size_t table_cap  = 0;       /* allocated capacity */

    static int q[16];                   /* denominator coefficients (q[0]=1) */
    static int c[16];                   /* recurrence coefficients (c[i] = -q[i]) */
    static int coeffs_computed = 0;     /* flag */

    /* --- Compute denominator and recurrence coefficients on first use */
    if (!coeffs_computed) {
        coeffs_computed = 1;

        /* local polynomial product */
        int poly[16] = {0};
        poly[0] = 1;
        int deg = 0;

        /* factors of the denominator: each defined by its coefficient list */
        static const int facs[][3] = {
            { 1,  -1,  0 },   /* 1 - x                       */
            { 1,   2,  0 },   /* 1 + 2x                      */
            { 1,  -5,  0 },   /* 1 - 5x                      */
            { 1,   1,  -1 },   /* 1 + x - x^2                 */
            { 1,   3,   1 },   /* 1 + 3x + x^2                */
            { 1,  -3,  -9 },   /* 1 - 3x - 9x^2               */
            { 1,  -4,  -1 },   /* 1 - 4x - x^2                */
            { 1,  -6,   4 },   /* 1 - 6x + 4x^2               */
            { 1,  -7,   1 }    /* 1 - 7x + x^2                */
        };
        static const int degs[9] = { 1, 1, 1, 2, 2, 2, 2, 2, 2 };

        for (int f = 0; f < 9; ++f) {
            int fdeg = degs[f];
            int newpoly[16] = {0};

            for (int i = 0; i <= deg; ++i) {
                for (int j = 0; j <= fdeg; ++j) {
                    newpoly[i + j] += poly[i] * facs[f][j];
                }
            }
            deg += fdeg;
            for (int i = 0; i <= deg; ++i) {
                poly[i] = newpoly[i];
            }
        }

        for (int i = 0; i <= 15; ++i) {
            q[i] = poly[i];
            c[i] = -q[i];          /* recurrence coefficient */
        }
    }

    /* --- Initialise first 15 terms (from OEIS) --------------------- */
    if (table_size == 0) {
        static const seq_t init[15] = {
            1,               /* a(0) */
            12,              /* a(1) */
            166,             /* a(2) */
            1784,            /* a(3) */
            17840,           /* a(4) */
            163504,          /* a(5) */
            1418549,         /* a(6) */
            11751784,        /* a(7) */
            94002810,        /* a(8) */
            730859800,       /* a(9) */
            5554472496,      /* a(10) */
            41437244784,     /* a(11) */
            304478259625,    /* a(12) */
            2209596042260,   /* a(13) */
            15871463933950   /* a(14) */
        };

        table_cap  = 15;
        table_size = 15;
        table = (seq_t *)malloc(table_cap * sizeof(seq_t));
        for (size_t i = 0; i < table_size; ++i) {
            table[i] = init[i];
        }
    }

    /* --- Return already computed value if available ----------------- */
    if (n < table_size) {
        return table[(size_t)n];
    }

    /* --- Ensure capacity for new values ---------------------------- */
    if (n + 1 > table_cap) {
        size_t new_cap = table_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        table = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
        table_cap = new_cap;
    }

    /* --- Compute missing terms using recurrence ------------------- */
    for (size_t k = table_size; k <= (size_t)n; ++k) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int i = 1; i <= 15; ++i) {
            int coeff = c[i];            /* may be positive or negative */
            if (coeff == 0) continue;
            seq_t term = table[(size_t)k - i] * (seq_t)((coeff > 0) ? coeff : -coeff);
            if (coeff > 0)
                pos += term;
            else
                neg += term;
        }
        /* pos >= neg for all k due to the recurrence; final term */
        table[(size_t)k] = pos - neg;
    }

    table_size = n + 1;
    return table[(size_t)n];
}
