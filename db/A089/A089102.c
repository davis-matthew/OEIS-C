/*
 * A089102  Sign twisted convoluted convolved Fibonacci numbers H_j^(6).
 * https://oeis.org/A089102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   poly_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A089102: Sign twisted convoluted convolved Fibonacci numbers H_j^(6)
   Formula: a(n) = 1/6 * (c1(n-1)+c2(n-1)-c3(n-1)-c4(n-1))
   where c1(n) is the coefficient of z^n in (1-z-z^2)^(-6),
         c2(n) in (1-z^2-z^4)^(-3),
         c3(n) in (1-z^3-z^6)^(-2),
         c4(n) in (1-z^6-z^12)^(-1).
   Each coefficient array is produced by the linear recurrence
              p0·c[n] + p1·c[n-1] + … + p12·c[n-12] = 0
   where pj are the coefficients of the corresponding base polynomial
   raised to the appropriate power.  (p0 = 1.)
   The recurrence requires only the 12 preceding terms, giving
   O(n) time per term.  The sequence is memoised so subsequent
   calls cost O(1).
   The algorithm works for any seq_t: unsigned integer types or an
   arbitrary‑precision class, because only +, -, *, / operations are
   used. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/poly_power.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Main function requested by the problem.  */
seq_t
a(idx_t n)
{
    if (n == 0)                         /* offset is 1 – return 0 for safety */
        return 0;

    /* static storage */
    static bool        init   = false;
    static seq_t      *c1 = NULL,
                      *c2 = NULL,
                      *c3 = NULL,
                      *c4 = NULL;
    static size_t      sz     = 0;     /* number of coefficients stored (≥1) */

    /* static coefficient polynomials for the four generating factors */
    static long long  poly1[13], poly2[13], poly3[13], poly4[13];
    static bool        poly_ok = false;

    /* -------------------------------------------------------------- */
    /*  initialise polynomials and first coefficient (index 0) once  */
    if (!poly_ok) {
        /* base polynomials (deg 2,4,6,12 respectively)                */
        static const long long base1[3] = {1, -1, -1};        /* 1 - z - z^2        */
        static const long long base2[5] = {1, 0, -1, 0, -1};  /* 1 - z^2 - z^4      */
        static const long long base3[7] = {1, 0, 0, -1, 0, 0, -1}; /* 1 - z^3 - z^6 */
        static const long long base4[13] = {
            1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1
        };                                        /* 1 - z^6 - z^12     */
        poly_power(base1, 3, 6, poly1);   /* (1 - z - z^2)^6     */
        poly_power(base2, 5, 3, poly2);   /* (1 - z^2 - z^4)^3   */
        poly_power(base3, 7, 2, poly3);   /* (1 - z^3 - z^6)^2   */
        poly_power(base4,13, 1, poly4);   /* (1 - z^6 - z^12)^1  */
        poly_ok = true;
    }

    /* -------------------------------------------------------------- */
    /*  ensure the coefficient tables contain at least n-1 indices   */
    if (!init) {
        sz = 1;                      /* store coefficient for z^0 */
        c1 = (seq_t *)malloc(sizeof(seq_t));
        c2 = (seq_t *)malloc(sizeof(seq_t));
        c3 = (seq_t *)malloc(sizeof(seq_t));
        c4 = (seq_t *)malloc(sizeof(seq_t));
        c1[0] = c2[0] = c3[0] = c4[0] = 1;   /* constant term of each series = 1 */
        init = true;
    }

    size_t need = (size_t)(n - 1) + 1;   /* number of coefficients needed   */
    if (need > sz) {                      /* expand arrays to required size */
        seq_t *new1 = (seq_t *)realloc(c1, sizeof(seq_t) * need);
        seq_t *new2 = (seq_t *)realloc(c2, sizeof(seq_t) * need);
        seq_t *new3 = (seq_t *)realloc(c3, sizeof(seq_t) * need);
        seq_t *new4 = (seq_t *)realloc(c4, sizeof(seq_t) * need);
        if (!new1 || !new2 || !new3 || !new4) abort(); /* memory error */
        c1 = new1; c2 = new2; c3 = new3; c4 = new4;

        /* compute new coefficients using the linear recurrence             */
        for (size_t k = sz; k < need; ++k) {
            seq_t val1 = 0, val2 = 0, val3 = 0, val4 = 0;
            /* recurrence: c[n] = -∑_{j=1..12} p[j] · c[n-j]                  */
            for (int j = 1; j <= 12 && j <= (int)k; ++j) {
                val1 -= (seq_t)poly1[j] * c1[k - j];
                val2 -= (seq_t)poly2[j] * c2[k - j];
                val3 -= (seq_t)poly3[j] * c3[k - j];
                val4 -= (seq_t)poly4[j] * c4[k - j];
            }
            c1[k] = val1;
            c2[k] = val2;
            c3[k] = val3;
            c4[k] = val4;
        }
        sz = need;
    }

    /* -------------------------------------------------------------- */
    /*  combine the four series according to the generating‑function    */
    seq_t res = c1[n - 1] + c2[n - 1] - c3[n - 1] - c4[n - 1];
    res /= 6;   /* division by six is exact by construction */
    return res;
}
