/*
 * A089095  Convoluted convolved Fibonacci numbers G_j^(8).
 * https://oeis.org/A089095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1/8 * ( [z^{n-1}] (1/(1 - z - z²)^8) - [z^{n-1}] (1/(1 - z² - z⁴)^4) )
 *
 * The coefficients of the two generating functions satisfy linear
 * recurrences with constant coefficients.  This implementation
 * computes them on demand and memoises the results in static arrays.
 */

#include <stdlib.h>   /* malloc, realloc */

static const int q[17] = {
    /* (1 - z - z²)^8  coefficients for the denominator */
    1,   /* z^0  */
   -8,   /* z^1  */
   20,   /* z^2  */
    0,   /* z^3  */
  -70,   /* z^4  */
   56,   /* z^5  */
  112,   /* z^6  */
 -120,   /* z^7  */
 -125,   /* z^8  */
  120,   /* z^9  */
  112,   /* z^10 */
  -56,   /* z^11 */
  -70,   /* z^12 */
    0,   /* z^13 */
   20,   /* z^14 */
    8,   /* z^15 */
    1    /* z^16 */
};

static const int r[17] = {
    /* (1 - z² - z⁴)^4  coefficients for the denominator */
    1,    /* z^0  */
    0,    /* z^1  */
   -4,    /* z^2  */
    0,    /* z^3  */
    2,    /* z^4  */
    0,    /* z^5  */
    8,    /* z^6  */
    0,    /* z^7  */
   -5,    /* z^8  */
    0,    /* z^9  */
   -8,    /* z^10 */
    0,    /* z^11 */
    2,    /* z^12 */
    0,    /* z^13 */
    4,    /* z^14 */
    0,    /* z^15 */
    1     /* z^16 */
};

static seq_t *b = NULL;      /* coefficients of (1/(1 - z - z²))^8   */
static seq_t *c = NULL;      /* coefficients of (1/(1 - z² - z⁴))^4   */
static size_t cache_len = 0; /* number of computed coefficients */

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; below is undefined */

    idx_t k = n - 1;               /* need coefficients b[k] and c[k] */

    /* enlarge the cache if required */
    if ((size_t)k >= cache_len) {
        size_t new_len = (size_t)k + 1;
        seq_t *new_b = (seq_t*)realloc(b, new_len * sizeof(seq_t));
        seq_t *new_c = (seq_t*)realloc(c, new_len * sizeof(seq_t));
        if (!new_b || !new_c) {
            /* Allocation failed – return zero to avoid UB */
            return 0;
        }
        /* Keep the original pointers only after successful reallocation */
        b = new_b;
        c = new_c;

        /* initialise new entries */
        for (idx_t i = cache_len; i <= k; ++i) {
            if (i == 0) {          /* base values */
                b[i] = (seq_t)1;
                c[i] = (seq_t)1;
            } else {
                /* recurrence for b[i]: sum_{j=1..16} q[j] * b[i-j] */
                seq_t sum_b = (seq_t)0;
                for (int j = 1; j <= 16 && (idx_t)j <= i; ++j) {
                    if (q[j] == 0) continue;
                    sum_b += (seq_t)q[j] * b[i - j];
                }
                b[i] = -sum_b;

                /* recurrence for c[i]: sum_{j=1..16} r[j] * c[i-j] */
                seq_t sum_c = (seq_t)0;
                for (int j = 1; j <= 16 && (idx_t)j <= i; ++j) {
                    if (r[j] == 0) continue;
                    sum_c += (seq_t)r[j] * c[i - j];
                }
                c[i] = -sum_c;
            }
        }
        cache_len = new_len;
    }

    /* a(n) = (b[k] - c[k]) / 8  */
    seq_t diff = b[k] - c[k];
    return diff / (seq_t)8;
}
