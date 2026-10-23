/*
 * A143356  A051731 * A006218.
 * https://oeis.org/A143356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A006218(d) where A006218(k) = Sum_{i=1..k} τ(i)
 *            and τ(i) is the number of divisors of i. Thus a(n) = Sum_{d|n}
 *            Sum_{i=1}^d τ(i)
 * Range:     u8 n <= 35, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *tau = NULL;   /* τ(i) for i up to current limit */
static seq_t *f   = NULL;   /* cumulative divisor counts f(i) = Σ_{k≤i} τ(k) */
static idx_t   max_n = 0;   /* highest index currently computed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; n=0 is unused but handle safely */

    /* extend precomputed arrays if necessary */
    if (n > max_n)
    {
        idx_t new_max = n;

        /* allocate new temporary arrays */
        seq_t *new_tau = (seq_t*)malloc((new_max + 1) * sizeof(seq_t));
        seq_t *new_f   = (seq_t*)malloc((new_max + 1) * sizeof(seq_t));

        /* initialize */
        for (idx_t i = 0; i <= new_max; ++i) new_tau[i] = 0;

        /* compute τ(i) by counting multiples */
        for (idx_t i = 1; i <= new_max; ++i)
            for (idx_t j = i; j <= new_max; j += i)
                new_tau[j] += 1;

        /* compute cumulative sums f(i) = Σ_{k≤i} τ(k) */
        new_f[0] = 0;
        for (idx_t i = 1; i <= new_max; ++i)
            new_f[i] = new_f[i-1] + new_tau[i];

        /* free old arrays */
        free(tau);
        free(f);

        /* install new arrays */
        tau = new_tau;
        f   = new_f;
        max_n = new_max;
    }

    /* compute a(n) = Σ_{d|n} f(d) by enumerating divisors */
    seq_t result = 0;
    for (idx_t d = 1; (idx_t)d * d <= n; ++d)
        if (n % d == 0)
        {
            result += f[d];
            idx_t other = n / d;
            if (other != d)
                result += f[other];
        }

    return result;
}
