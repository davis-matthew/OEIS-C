/*
 * A041592  Numerators of continued fraction convergents to sqrt(314).
 * https://oeis.org/A041592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 43, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041592: Numerators of continued fraction convergents to sqrt(314).
   Recurrence:  p_{-2}=0, p_{-1}=1,
                a0 = floor(sqrt(314)) = 17,
                for k≥0:
                a_k   = a0        (k==0)
                        period[(k-1) mod L]  (k≥1)
                p_k   = a_k * p_{k-1} + p_{k-2}.
   Here period is the repeating part of the continued fraction of sqrt(314),
   of length L = 7: [1,2,1,1,2,1,34].
   The function caches computed values so that calls in any order are
   efficient. */

#include <stdlib.h>

static const uint32_t D_CONST = 314U;
static const uint32_t A0_CONST = 17U;  /* floor(sqrt(314)) */

/* Cached data */
static seq_t *numerators = NULL;        /* array of p_k values */
static size_t num_cap    = 0;           /* capacity of the array */
static size_t num_size   = 0;           /* number of values stored */

static uint16_t *period   = NULL;       /* repeating part of CF */
static size_t    period_len = 0;        /* length of the period */

static void init_period(void)
{
    /* Compute the periodic part of the continued fraction for sqrt(314).
       The sequence starts with a0 = 17, followed by the periodic part
       until d_k == 1 (which produces the final 2*a0). */
    unsigned long long a0 = A0_CONST;
    unsigned long long m  = 0;
    unsigned long long d  = 1;
    unsigned long long a  = a0;

    /* Dynamic array for period elements */
    size_t cap = 8;
    period = (uint16_t *)malloc(sizeof(uint16_t) * cap);
    size_t idx  = 0;

    while (1)
    {
        m = d * a - m;
        d = (D_CONST - m * m) / d;
        a = (a0 + m) / d;  /* next partial quotient */

        if (idx >= cap)
        {
            cap <<= 1;
            period = (uint16_t *)realloc(period, sizeof(uint16_t) * cap);
        }
        period[idx++] = (uint16_t)a;

        if (d == 1)          /* period ends when d_k == 1 */
            break;
    }
    period_len = idx;
}

static uint16_t get_a_k(idx_t k)
{
    if (k == 0)
        return (uint16_t)A0_CONST;
    /* period[(k-1) % period_len] */
    return period[(k - 1) % period_len];
}

seq_t a(idx_t n)
{
    /* Initialize period on first call */
    if (!period)
        init_period();

    /* Grow numerators array if needed */
    if (num_size <= n)
    {
        /* Resize capacity to at least n+1 */
        size_t new_cap = num_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        numerators = (seq_t *)realloc(numerators, sizeof(seq_t) * new_cap);
        num_cap = new_cap;

        /* Compute missing convergents up to index n */
        while (num_size <= n)
        {
            idx_t k = num_size;                 /* current index to compute */
            uint16_t ak = get_a_k(k);
            seq_t pk;

            if (k == 0)
                pk = (seq_t)ak;                       /* a0 */
            else if (k == 1)
                pk = (seq_t)ak * numerators[0] + (seq_t)1;
            else
                pk = (seq_t)ak * numerators[k - 1] + numerators[k - 2];

            numerators[num_size++] = pk;
        }
    }

    return numerators[n];
}
