/*
 * A042246  Numerators of continued fraction convergents to sqrt(649).
 * https://oeis.org/A042246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = the n-th numerator of the continued‑fraction convergents
 *            to sqrt(649)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 16, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=25; for n≥1, a(n)=coeff_n*a(n-1)+a(n-2), where coeff_n are the continued‑fraction coefficients of sqrt(649). */
#include <stdlib.h>
#include <stdint.h>

/* stores the periodic part of the continued‑fraction coefficients of sqrt(649) */
static uint64_t *period = NULL;
static size_t period_len = 0;
static size_t period_cap = 0;

/* initialise the period array once */
static void init_period(void)
{
    if (period_len != 0)          /* already initialised */
        return;

    const uint64_t D = 649;
    const uint64_t a0 = 25;       /* floor(sqrt(D)) */

    uint64_t m = 0, d = 1, a = a0;
    size_t cap = 64;
    period = (uint64_t*)malloc(cap * sizeof(uint64_t));
    if (!period)
        exit(1);
    period_cap = cap;
    period_len = 0;

    do
    {
        /* compute next triple (m,d,a) */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        if (period_len >= period_cap)
        {
            size_t newcap = period_cap * 2;
            uint64_t *tmp = (uint64_t*)realloc(period, newcap * sizeof(uint64_t));
            if (!tmp)
                exit(1);
            period = tmp;
            period_cap = newcap;
        }
        period[period_len++] = a;
    } while (a != 2 * a0);   /* include the last coefficient 2*a0 */
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* make sure the period is known */
    init_period();

    /* grow cache if necessary */
    if ((size_t)n >= cache_cap)
    {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            exit(1);
        cache = tmp;
        cache_cap = newcap;
    }

    /* fill cache up to index n */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)25;
        }
        else if (i == 1)
        {
            seq_t coeff = (seq_t)period[0];
            cache[1] = coeff * cache[0] + (seq_t)1;
        }
        else
        {
            seq_t coeff = (seq_t)period[(i - 1) % period_len];
            cache[i] = coeff * cache[i - 1] + cache[i - 2];
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
