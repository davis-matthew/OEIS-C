/*
 * A128778  a(n) = numerator of Product_{k=1..n} (1 + {n/k}), where {x} is the
 *          fractional part of x, {x} = x - floor(x).
 * https://oeis.org/A128778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of ∏_{k=1..n} (1 + {n/k}), where {x} = x − ⌊x⌋
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* Smallest–prime–factor table and exponent differences.  */
static idx_t *spf = NULL;      /* spf[i] = smallest prime factor of i (0 if i < 2) */
static int   *expo = NULL;     /* expo[p] = exponent difference for prime p */
static idx_t  spf_limit = 0;   /* current size of the tables (high‑value included) */

/* Build or enlarge tables up to @limit (inclusive). */
static void init_factorization(idx_t limit)
{
    if (limit <= spf_limit)
        return;                   /* already large enough */

    free(spf);   /* free old allocation if any */
    free(expo);

    /* Allocate new tables.  The casts are needed for C++ compilation. */
    spf  = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    expo = (int   *)malloc((limit + 1) * sizeof(int));
    if (!spf || !expo)            /* allocation failed */
        abort();

    spf_limit = limit;

    /* Initialise spf array. */
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = 0;

    /* Simple sieve for smallest prime factor. */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = i;
            if (i <= limit / i) {         /* avoid overflow when i*i > limit */
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }
}

/* Factorise @val and add (+1) or subtract (–1) its exponents into expo. */
static void factor_val(idx_t val, int direction)
{
    while (val > 1) {
        idx_t p = spf[val];
        expo[p] += direction;
        val /= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                    /* offset is 1 – n==0 never used */
        return (seq_t)1;

    /* All numbers that appear are < 2*n, so this covers them. */
    idx_t limit = 2 * n;
    init_factorization(limit);
    memset(expo, 0, (limit + 1) * sizeof(int));

    for (idx_t k = 1; k <= n; ++k) {
        idx_t r = n % k;                 /* fractional part denominator */
        idx_t num = k + r;                /* numerator of current factor */
        factor_val(num, +1);              /* add numerator factors */
        factor_val(k,    -1);              /* subtract denominator factors */
    }

    seq_t result = (seq_t)1;
    for (idx_t p = 2; p <= limit; ++p) {
        int e = expo[p];
        if (e > 0) {
            seq_t power = seq_pow((seq_t)p, (idx_t)e);
            result = seq_mul(result, power);
        }
    }
    return result;
}
