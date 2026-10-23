/*
 * A193263  G.f.: A(x) = x + Sum_{n>=1} x^(2*n) / (1+x)^A193259(n).
 * https://oeis.org/A193263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 29, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A193263
 * a(1) = 1
 * a(n) = Σ_{m=1}^{⌊n/2⌋} (-1)^{n-2m} * binom(A193259(m)+n-2m-1, n-2m)
 * where A193259(m) = m + ⌊log₂ m⌋ + A011371(m)
 * and A011371(m) = exponent of 2 in m! (sum of trailing‑zero counts).
 */

#include <stdlib.h>
#include <limits.h>
#include "../utils/binom.h"

static idx_t *c_arr     = NULL;   /* A193259(m) */
static idx_t *v2fac     = NULL;   /* exponent of 2 in m!   */
static idx_t *log2_arr  = NULL;   /* ⌊log₂ m⌋              */
static size_t cap       = 0;       /* allocated size of the arrays */

/* ensure the tables are built up to max_needed (inclusive) */
static void ensure_tables(idx_t max_needed)
{
    if ((idx_t)cap > max_needed) return; /* already enough */

    size_t newcap = cap ? (cap << 1) : 1;
    if (newcap < (size_t)max_needed + 1) newcap = (size_t)max_needed + 1;
    if (newcap < cap) newcap = SIZE_MAX; /* overflow guard */

    /* realloc with explicit casts for C++ */
    c_arr     = (idx_t *)realloc(c_arr,     newcap * sizeof(idx_t));
    v2fac     = (idx_t *)realloc(v2fac,     newcap * sizeof(idx_t));
    log2_arr  = (idx_t *)realloc(log2_arr,  newcap * sizeof(idx_t));

    for (size_t i = cap; i < newcap; ++i) {
        idx_t ii = (idx_t)i;
        if (i == 0) {
            v2fac[i]   = 0;
            log2_arr[i] = 0;
            c_arr[i]   = 0;
        } else {
            /* trailing‑zero count of i  (exponent of 2 in i) */
            uint64_t tz = __builtin_ctzll(ii);
            v2fac[i]   = v2fac[i - 1] + tz;
            /* floor(log₂ i) */
            log2_arr[i] = 63 - __builtin_clzll(ii);
            /* A193259(i) = i + ⌊log₂ i⌋ + exponent of 2 in i! */
            c_arr[i]   = ii + log2_arr[i] + v2fac[i];
        }
    }
    cap = newcap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1 */
    if (n == 1) return 1;          /* the initial x term */

    idx_t half = n >> 1;           /* floor(n/2) */
    ensure_tables(half);

    seq_t result = 0;
    for (idx_t m = 1; m <= half; ++m) {
        idx_t j = n - 2 * m;                       /* exponent in expansion */
        idx_t C = c_arr[m] + j - 1;                /* A193259(m)+j-1   */
        seq_t bin = binom(C, j);                   /* binomial coeff.  */
        if (j & 1) bin = -bin;                     /* (-1)^j           */
        result += bin;
    }
    return result;
}
