/*
 * A168243  Expansion of e.g.f. Product_{i>=1} (1 + x^i)^(1/i).
 * https://oeis.org/A168243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=1}^{n} f(k) * a(n-k) * * * product_{t=n-k+1}^{n-1}
 *            t * where f(k) = sum_{d|k} (-1)^{k/d-1} * and a(0) = 1. (OEIS
 *            A168243)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- static helper data ---------- */
static seq_t *cache = NULL;      /* store computed a(i) */
static size_t cache_cap = 0;     /* allocated capacity of cache   */
static idx_t cache_max = 0;      /* highest index computed so far */

static int *f_arr = NULL;       /* array of f(k) values          */
static idx_t f_cap = 0;         /* capacity of f_arr             */

/* ---------- recompute f(k) for 0..N ---------- */
static void recompute_f_arr(idx_t N)
{
    /* expand array if needed */
    if (f_cap < N + 1) {
        int *tmp = (int *)realloc(f_arr, (N + 1) * sizeof(int));
        if (tmp == NULL) abort();
        f_arr = tmp;
        f_cap = N + 1;
    }

    /* initialise to zero */
    for (idx_t i = 0; i <= N; ++i) f_arr[i] = 0;

    /* compute f(k) by divisor enumeration */
    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t j = i; j <= N; j += i) {
            idx_t t = j / i;                    /* j = i * t  */
            int sign = ((t - 1) & 1) ? -1 : 1;  /* (-1)^{t-1} */
            f_arr[j] += sign;
        }
    }
}

/* ---------- value computation ---------- */
seq_t a(idx_t n)
{
    /* ensure cache can hold up to n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        /* initialise new entries to zero */
        for (idx_t i = cache_cap; i <= n; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise a(0) */
    if (cache[0] == (seq_t)0) cache[0] = (seq_t)1;

    /* compute missing f(k) values up to n */
    if (f_cap <= n) recompute_f_arr(n);

    /* compute required a(i) values */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        seq_t sum_pos = (seq_t)0;   /* positive contributions */
        seq_t sum_neg = (seq_t)0;   /* negative contributions */
        seq_t p = (seq_t)1;         /* product of (i-k+1) terms */

        for (idx_t k = 1; k <= i; ++k)
        {
            int fk = f_arr[k];
            if (fk != 0) {
                /* term = fk * a[i-k] * p * |fk| */
                seq_t temp = cache[i - k] * p;          /* a[i-k] * p  */
                seq_t abs_fk = (fk > 0) ? (seq_t)fk : (seq_t)(-fk);
                temp = temp * abs_fk;
                if (fk > 0)
                    sum_pos += temp;
                else
                    sum_neg += temp;
            }

            /* update p for next k (unless this is the last one) */
            if (k < i) {
                seq_t factor = (seq_t)(i - k);
                p = p * factor;
            }
        }

        /* a[i] = sum_pos - sum_neg  (guaranteed non‑negative) */
        cache[i] = sum_pos - sum_neg;
    }

    cache_max = n;
    return cache[n];
}
