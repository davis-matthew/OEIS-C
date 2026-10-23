/*
 * A014570  Inverse of 561st cyclotomic polynomial.
 * https://oeis.org/A014570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in 1/Φ_561(x), periodic with period
 *            561. The coefficients are obtained by solving the linear
 *            recurrence (x^3-1)(x^11-1)(x^17-1)(x^561-1) · A(x) =
 *            (x-1)(x^33-1)(x^51-1)(x^187-1), where A(x)=∑a_k x^k.
 *            Coefficients of the two polynomials are ±1, so the recurrence is
 *            a_k = N...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define PERIOD 561
/* highest possible exponent in the denominator polynomial */
#define MAX_DEG 593

static seq_t denom[MAX_DEG];
static seq_t numer[MAX_DEG];
static seq_t cache[PERIOD];
static bool initialized = false;

/* compute the coefficient arrays of the two polynomials */
static void init_polys(void)
{
    /* denominator: (x^3-1)(x^11-1)(x^17-1)(x^561-1) */
    const int d_exps[4] = {3, 11, 17, 561};
    int cur = 0;
    seq_t prod[MAX_DEG] = {0};
    prod[0] = 1;

    for (int f = 0; f < 4; ++f) {
        int e = d_exps[f];
        seq_t next[MAX_DEG] = {0};
        for (int i = 0; i <= cur; ++i) {
            if (prod[i] == 0) continue;
            next[i]     += prod[i] * (-1);   /* constant term of (x^e-1) */
            next[i + e] += prod[i];          /* x^e term          */
        }
        cur += e;
        for (int i = 0; i <= cur; ++i) prod[i] = next[i];
    }
    for (int i = 0; i <= cur; ++i) denom[i] = prod[i];
    for (int i = cur + 1; i < MAX_DEG; ++i) denom[i] = 0;

    /* numerator: (x-1)(x^33-1)(x^51-1)(x^187-1) */
    const int n_exps[4] = {1, 33, 51, 187};
    cur = 0;
    prod[0] = 1;
    for (int f = 0; f < 4; ++f) {
        int e = n_exps[f];
        seq_t next[MAX_DEG] = {0};
        for (int i = 0; i <= cur; ++i) {
            if (prod[i] == 0) continue;
            next[i]     += prod[i] * (-1);
            next[i + e] += prod[i];
        }
        cur += e;
        for (int i = 0; i <= cur; ++i) prod[i] = next[i];
    }
    for (int i = 0; i <= cur; ++i) numer[i] = prod[i];
    for (int i = cur + 1; i < MAX_DEG; ++i) numer[i] = 0;
}

/* initialise cache with the first PERIOD terms */
static void init_cache(void)
{
    if (initialized) return;
    init_polys();

    seq_t a[PERIOD];
    a[0] = numer[0];            /* = 1 */
    for (int k = 1; k < PERIOD; ++k) {
        seq_t sum = 0;
        for (int i = 1; i <= k; ++i) {
            if (denom[i] != 0) sum += denom[i] * a[k - i];
        }
        a[k] = numer[k] - sum;
    }
    for (int i = 0; i < PERIOD; ++i) cache[i] = a[i];
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_cache();
    return cache[(size_t)(n % PERIOD)];
}
