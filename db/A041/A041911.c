/*
 * A041911  Denominators of continued fraction convergents to sqrt(477).
 * https://oeis.org/A041911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Denominators of continued‑fraction convergents to sqrt(477)
   Computed by the recurrence
     q_{-2} = 1 ,  q_{-1} = 0 ,
     q_n   = a_n * q_{n-1} + q_{n-2}
   where a_0 = 21 and for n ≥ 1 the coefficients a_n repeat the period
   [1, 5, 3, 1, 4, 10, 1, 2, 2, 4, 2, 2, 1, 10, 4, 1, 3, 5, 1, 42]. */

#include <stdlib.h>

static const unsigned int periodCoeffs[20] = {
    1, 5, 3, 1, 4, 10, 1, 2, 2, 4,
    2, 2, 1, 10, 4, 1, 3, 5, 1, 42
};

static seq_t *denom_cache = NULL;
static idx_t denom_cache_len = 0;
static size_t denom_cache_cap = 0;

/* Return the continued‑fraction coefficient a_n for sqrt(477). */
static inline seq_t cf_coeff(idx_t n)
{
    if (n == 0)
        return (seq_t)21;          /* a_0 */
    size_t pos = (size_t)n - 1u;   /* coefficients start at a_1 */
    pos %= 20u;                    /* period length = 20 */
    return (seq_t)periodCoeffs[pos];
}

seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if ((size_t)n >= denom_cache_cap)
    {
        size_t new_cap = denom_cache_cap ? denom_cache_cap * 2 : 1;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(denom_cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;      /* allocation failure – undefined for us */
        denom_cache = tmp;
        denom_cache_cap = new_cap;
    }

    /* Compute any missing denominators up to index n */
    for (idx_t i = denom_cache_len; i <= n; ++i)
    {
        seq_t coeff = cf_coeff(i);
        if (i == 0)
        {
            denom_cache[0] = (seq_t)1;
        }
        else if (i == 1)
        {
            denom_cache[1] = coeff * denom_cache[0];  /* + 0 from q_{-1} */
        }
        else
        {
            denom_cache[i] = coeff * denom_cache[i - 1] + denom_cache[i - 2];
        }
    }
    denom_cache_len = n + 1;
    return denom_cache[n];
}
