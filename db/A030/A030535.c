/*
 * A030535  Expansion of Molien series for 16-D extraspecial group 2^{1+2*4}.
 * https://oeis.org/A030535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Coefficient of x^n in (1 -7*x +63*x^2 -161*x^3 +1394*x^4
 *            -307*x^5 +7665*x^6 +987*x^7 +13498*x^8 +987*x^9 +7665*x^10
 *            -307*x^11 +1394*x^12 -161*x^13 +63*x^14 -7*x^15 +x^16) / ((1 -
 *            x)^8 * (1 - x^2)^8)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 16, u64 n <= 85, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed for dynamic allocation and basic types */
#include <stdlib.h>
#include <stdbool.h>

/* ---------------  Global data  --------------- */

static seq_t *y_cache = NULL;      /* y[n] = coefficient of 1 / [(1-x)^16(1+x)^8] */
static idx_t y_max  = 0;           /* largest n for which y_cache[n] is computed */
static size_t  y_cap  = 0;         /* current capacity of y_cache */
static int D[25] = {0};            /* coefficients of (1-x)^16(1+x)^8 */
static bool D_init = false;

/* ---------------  Helper: compute D  --------------- */
static void init_D(void)
{
    if (D_init) return;
    int binom16[17];
    binom16[0] = 1;
    for (int i = 1; i <= 16; ++i)
        binom16[i] = binom16[i-1] * (16 - i + 1) / i;

    int binom8[9];
    binom8[0] = 1;
    for (int i = 1; i <= 8; ++i)
        binom8[i] = binom8[i-1] * (8 - i + 1) / i;

    for (int k = 0; k <= 24; ++k) D[k] = 0;
    for (int i = 0; i <= 16; ++i) {
        int sign = (i % 2 == 0) ? 1 : -1;
        int ci = binom16[i];
        for (int j = 0; j <= 8; ++j) {
            int cj = binom8[j];
            int prod = sign * ci * cj;
            D[i + j] += prod;
        }
    }
    D_init = true;
}

/* ---------------  Ensure y_cache up to n  --------------- */
static void ensure_y(idx_t n)
{
    if (!y_cache) {
        y_cap = 16;
        y_cache = (seq_t *)malloc(y_cap * sizeof(seq_t));
        if (!y_cache) abort();                /* allocation failure */
        y_cache[0] = (seq_t)1;                /* y[0] = 1 */
        y_max = 0;
    }

    if (n <= y_max) return;                  /* already computed */

    /* possibly grow the cache */
    size_t need_cap = (size_t)(n + 1);
    if (need_cap > y_cap) {
        size_t new_cap = y_cap;
        while (new_cap < need_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(y_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        y_cache = tmp;
        y_cap = new_cap;
    }

    init_D();                                 /* make sure D is ready */

    /* compute y[n] progressively using the recurrence:
       For n >= 1:  y[n] = (- Σ_{i=1}^{24} D[i] * y[n-i]) / D[0]
       Since D[0] == 1, this simplifies to y[n] = Σ_{i>0} |D[i]|*y[n-i] with
       appropriate sign handling.  */
    for (idx_t m = y_max + 1; m <= n; ++m) {
        seq_t pos = 0;   /* sum of terms where D[i] > 0 */
        seq_t neg = 0;   /* sum of terms where D[i] < 0 */
        for (int i = 1; i <= 24 && (idx_t)i <= m; ++i) {
            int di = D[i];
            seq_t term = y_cache[m - (idx_t)i] * (seq_t)(di > 0 ? di : -di);
            if (di > 0) pos += term;
            else       neg += term;
        }
        y_cache[m] = neg - pos;                /* neg >= pos by construction */
    }
    y_max = n;
}

/* ---------------  Main sequence function  --------------- */
seq_t a(idx_t n)
{
    /* Numerator coefficients of the generating function */
    static const int num[17] = {
        1, -7, 63, -161, 1394, -307, 7665, 987,
        13498, 987, 7665, -307, 1394, -161, 63, -7, 1
    };

    ensure_y(n);                     /* make sure y_cache[0..n] exist */

    seq_t pos = 0;   /* sum of positive contributions */
    seq_t neg = 0;   /* sum of negative contributions */

    for (int t = 0; t <= 16; ++t) {
        if ((idx_t)t > n) break;
        seq_t term = y_cache[n - (idx_t)t] *
                     (seq_t)(num[t] > 0 ? num[t] : -num[t]);
        if (num[t] >= 0) pos += term;
        else             neg += term;
    }
    return pos - neg;                     /* final value is non‑negative */
}
