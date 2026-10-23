/*
 * A014620  Inverse of 611th cyclotomic polynomial.
 * https://oeis.org/A014620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014620: Inverse of 611th cyclotomic polynomial.
   The coefficients a(n) of the formal power series 1/Φ_611(x) satisfy
   the recurrence                                     a(0) = 1
   a(n) = -∑_{k=1}^{min(552,n)} c[k] · a(n−k) for n≥1,
   where c[k] (k=0..552) are the coefficients of Φ_611(x).
   The recurrence guarantees the periodicity of period 611. */

#include <stdlib.h>

static int init_done = 0;

/* coefficients of Φ_611(x) (degree 552) */
static seq_t c[553];

/* cached a(n) values */
static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;     // highest index currently stored

/* initialise the polynomial coefficients c[0..552] once */
static void init_c(void) {
    int i;
    /* D[j] is the coefficient of x^j in (x^47-1)(x^13-1)
       D[0] = 1, D[13] = -1, D[47] = -1, D[60] = 1, others 0 */
    for (i = 0; i <= 552; ++i) {
        seq_t sum = 0;
        if (i >= 13)  sum += -c[i - 13];   /* D[13] = -1 */
        if (i >= 47)  sum += -c[i - 47];   /* D[47] = -1 */
        if (i >= 60)  sum +=  c[i - 60];   /* D[60] = 1  */

        seq_t Ni = 0;
        if (i == 0)    Ni = 1;             /* N[0] = 1 */
        else if (i == 1) Ni = -1;          /* N[1] = -1 */
        /* N[i] = 0 for 2 ≤ i ≤ 552 */

        c[i] = Ni - sum;                   /* c[0] = 1 automatically   */
    }
    init_done = 1;
}

/* compute a(n) recursively up to the requested index */
static void extend_cache(idx_t n) {
    if (!init_done) init_c();

    /* allocate or grow the cache */
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (a_cache == NULL) return;      /* allocation failure – unlikely */
        a_cache[0] = 1;                    /* a(0) = 1 */
        a_cache_len = 0;
    } else if (n > a_cache_len) {
        a_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
        if (a_cache == NULL) return;      /* allocation failure – unlikely */
    }

    /* fill missing terms */
    for (idx_t i = a_cache_len + 1; i <= n; ++i) {
        seq_t sum = 0;
        size_t kmax = (i <= 552) ? (size_t)i : 552;
        for (size_t k = 1; k <= kmax; ++k)
            sum += c[k] * a_cache[i - k];
        a_cache[i] = -sum;
    }
    a_cache_len = (size_t)n;
}

seq_t a(idx_t n) {
    extend_cache(n);
    return a_cache[(size_t)n];
}
