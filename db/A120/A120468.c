/*
 * A120468  Expansion of -2*x*(-8-12*x+9*x^2) / ( (x-1)*(3*x-1)*(3*x+1)*(1+x)
 *          ).
 * https://oeis.org/A120468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   For even n: a(n) = 3^(n+1) - 3 For odd n: a(n) = (7*3^(n+1) + 1)
 *            / 4 This follows from the OEIS definition 8*a(n) =
 *            (-1)^(n+1)*(13+3^(n+2)) + 11*(3^(n+1)-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *pow3_cache = NULL;   /* pow3_cache[i] = 3^i */
static idx_t pow3_len   = 0;        /* current allocated size */

/* Ensure that 3^(0..m) are computed and stored in pow3_cache */
static void ensure_pow3(idx_t m)
{
    if (pow3_len > m)
        return;

    /* Reallocate once for all new entries to reduce realloc calls */
    idx_t new_len = m + 1;
    seq_t *new_cache = (seq_t *)realloc(pow3_cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort(); /* memory failure */
    pow3_cache = new_cache;

    for (idx_t i = pow3_len; i <= m; ++i) {
        if (i == 0)
            pow3_cache[i] = (seq_t)1;
        else
            pow3_cache[i] = pow3_cache[i - 1] * (seq_t)3;
    }
    pow3_len = new_len;
}

seq_t a(idx_t n)
{
    /* we need 3^(n+1) -> ensure indices up to n+1 are present */
    ensure_pow3(n + 1);
    seq_t p = pow3_cache[n + 1];

    if ((n & (idx_t)1) == 0) { /* even n */
        /* a(n) = p - 3 */
        return p - (seq_t)3;
    } else {                  /* odd n */
        /* a(n) = (7*p + 1) / 4 */
        return ( (seq_t)7 * p + (seq_t)1 ) / (seq_t)4;
    }
}
