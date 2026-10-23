/*
 * A042453  Denominators of continued fraction convergents to sqrt(754).
 * https://oeis.org/A042453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued‑fraction convergents to sqrt(754).
   For sqrt(D) the convergent denominators q_n satisfy:
       q_{-1} = 0,  q_0 = 1
       q_n   = a_n * q_{n-1} + q_{n-2}  for n ≥ 1
   where a_0 = floor(sqrt(D)) and a_n (n ≥ 1) are the periodic
   coefficients of the continued fraction for sqrt(D). */

#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Periodic continued fraction coefficients for sqrt(754)                */
/* --------------------------------------------------------------------- */

#define MAX_PERIOD 512

static int period_a[MAX_PERIOD];
static unsigned period_len = 0;
static int period_inited = 0;

static void init_period(void)
{
    if (period_inited) return;

    const unsigned long long D   = 754ULL;   /* D > 0, non‑square           */
    const unsigned long long a0  = 27ULL;    /* floor(sqrt(D))              */

    unsigned long long m = 0;                /* m_0 */
    unsigned long long d = 1;                /* d_0 */
    unsigned long long a = a0;               /* a_0 */

    period_len = 0;
    /* Generate a_1 … a_{period_len} until a_k == 2*a0 (usual period end). */
    do
    {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        if (period_len < MAX_PERIOD)
            period_a[period_len++] = (int)a;
    } while (a != 2 * a0);

    period_inited = 1;
}

/* --------------------------------------------------------------------- */
/* Cache of computed denominators q_n                                   */
/* --------------------------------------------------------------------- */

static seq_t *q_cache = NULL;    /* q_cache[n] holds q_n for 0 ≤ n ≤ max_cached */
static size_t cache_len = 0;     /* current allocated capacity of q_cache     */
static idx_t max_cached = (idx_t)-1; /* highest n for which q_cache[n] is ready */

static void ensure_cache(idx_t n)
{
    if (max_cached != (idx_t)-1 && max_cached >= n) return;

    size_t needed = (size_t)n + 1;
    if (q_cache == NULL)
    {
        q_cache = (seq_t*)malloc(needed * sizeof(seq_t));
        cache_len = needed;
    }
    else if (needed > cache_len)
    {
        q_cache = (seq_t*)realloc(q_cache, needed * sizeof(seq_t));
        cache_len = needed;
    }

    if (max_cached == (idx_t)-1)       /* initialise the first term            */
    {
        q_cache[0] = 1;                 /* q_0 = 1                               */
        max_cached = 0;
    }

    for (idx_t k = max_cached + 1; k <= n; ++k)
    {
        int a_k;
        if (k == 1)
            a_k = period_a[0];          /* a_1                           */
        else
            a_k = period_a[(k - 1) % period_len];

        seq_t factor = (seq_t)a_k;

        if (k == 1)
            q_cache[1] = factor * q_cache[0];   /* q_1 = a_1*q_0 */
        else
            q_cache[k] = factor * q_cache[k-1] + q_cache[k-2];
    }
    max_cached = n;
}

/* --------------------------------------------------------------------- */
/* Public entry point required by the framework                          */
/* --------------------------------------------------------------------- */

/* External linkage as required. */
seq_t a(idx_t n)
{
    init_period();                /* generate periodic coefficients once */
    if (n == 0) return 1;         /* q_0 = 1 is trivial                 */
    ensure_cache(n);
    return q_cache[n];
}
