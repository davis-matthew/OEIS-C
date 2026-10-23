/*
 * A042253  Denominators of continued fraction convergents to sqrt(652).
 * https://oeis.org/A042253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent to sqrt(652)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 23, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        a(0) = 1, a(1) = 1, and for n ≥ 2 */
/*        a(n) = coeff(n) * a(n-1) + a(n-2)                       */
/*        where coeff(n) are the partial quotients of the continued fraction of sqrt(652): */
/*        1,1,6,1,3,1,3,2,5,1,16,5,1,1,1,1,1,2,12,2,1,1,1,1,5,16,1,5,2,3,1,3,6,1,1,1,50,… (period 36) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* constants */
static const idx_t D = 652;
static const idx_t ROOT = 25;           /* floor(sqrt(D)) */

/* period of continued fraction of sqrt(D) */
static seq_t period[256];
static size_t period_len = 0;
static bool period_ready = false;

static void compute_period(void)
{
    if (period_ready) return;
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = ROOT;
    do
    {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (ROOT + m) / d;
        period[period_len++] = (seq_t)a;
    } while (a != ROOT * 2);
    period_ready = true;
}

/* memoised denominators of convergents */
static seq_t *denoms   = NULL;
static size_t denoms_cap = 0;
static idx_t   denoms_max = 0;   /* highest index already computed */

/* ensure storage for indices up to n */
static void ensure_denoms(size_t n)
{
    if (denoms_cap > n) return;
    size_t new_cap = denoms_cap ? denoms_cap : 8;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(denoms, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);           /* allocation failure */
    if (!denoms)
        memset(tmp, 0, new_cap * sizeof(seq_t));
    else
        memset(tmp + denoms_cap, 0, (new_cap - denoms_cap) * sizeof(seq_t));
    denoms = tmp;
    denoms_cap = new_cap;
}

/* required function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;      /* a(0) = 1 */

    compute_period();                  /* ensure period is ready */
    ensure_denoms((size_t)n);          /* grow memo table if required */

    if (denoms_max < 1)                /* initialise first two values */
    {
        denoms[0] = (seq_t)1;
        denoms[1] = period[0] * denoms[0];  /* coeff a1 = period[0] */
        denoms_max = 1;
    }

    for (idx_t i = denoms_max + 1; i <= n; ++i)   /* compute missing denominators */
    {
        seq_t coeff = period[(size_t)((i - 1) % period_len)];
        denoms[i] = coeff * denoms[i - 1] + denoms[i - 2];
    }
    denoms_max = n;

    return denoms[n];
}
