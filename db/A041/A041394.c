/*
 * A041394  Numerators of continued fraction convergents to sqrt(212).
 * https://oeis.org/A041394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 51, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041394: Numerators of continued‑fraction convergents to sqrt(212) */
/* Recurrence:  P_{-2}=0, P_{-1}=1,  P_n = a_n*P_{n-1}+P_{n-2}   */
/* a_0 = floor(sqrt(212)) = 14, partial quotients a_1,a_2,... are periodic */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static data so that the function can be called in any order */
    static seq_t *values = NULL;      /* cache of numerators P_n */
    static size_t values_cap = 0;     /* capacity of the cache */
    static idx_t values_len = 0;      /* number of computed terms */

    static seq_t *periodic = NULL;   /* a_1 … a_period   */
    static size_t period_len = 0;     /* period length    */

    /* local constants */
    const seq_t D   = (seq_t)212;            /* number whose sqrt is taken */
    const seq_t a0  = (seq_t)14;             /* floor(sqrt(212))  */
    const seq_t a0x2 = a0 * (seq_t)2;        /* 2*a0 = 28, end of period */

    /* ---- initialise the periodic part of the continued fraction once ---- */
    if (!periodic) {
        /* use the standard algorithm for sqrt(D) */
        seq_t m   = (seq_t)0;
        seq_t d   = (seq_t)1;
        seq_t prev = a0;   /* this is a_0 */
        size_t cap = 16;
        periodic = (seq_t*)malloc(cap * sizeof(seq_t));
        size_t cnt = 0;

        while (1) {
            seq_t new_m = d * prev - m;
            seq_t new_d = (D - new_m * new_m) / d;
            seq_t a_k = (a0 + new_m) / new_d;   /* floor division */

            if (cnt >= cap) {
                cap <<= 1;
                periodic = (seq_t*)realloc(periodic, cap * sizeof(seq_t));
            }
            periodic[cnt++] = a_k;

            if (a_k == a0x2)                 /* period completed */
                break;

            m = new_m;
            d = new_d;
            prev = a_k;
        }
        period_len = cnt;     /* number of a_1 … a_period values */
    }

    /* ---- extend the cache of numerators if needed ---- */
    if (n >= values_len) {
        /* increase capacity if necessary */
        size_t new_cap = values_cap;
        if (new_cap == 0) new_cap = 16;
        while (new_cap <= n) new_cap <<= 1;

        if (new_cap > values_cap) {
            values = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
            values_cap = new_cap;
        }

        /* compute terms from values_len up to n (inclusive) */
        for (idx_t i = values_len; i <= n; ++i) {
            seq_t ai;
            if (i == 0) {
                ai = a0;
                values[0] = ai;   /* P_0 = a_0 */
            } else if (i == 1) {
                ai = periodic[0];                /* a_1 */
                values[1] = ai * values[0] + (seq_t)1;  /* P_1 = a_1*P_0 + P_{-1} */
            } else {
                ai = periodic[(i-1) % period_len];      /* a_i for i>=2 */
                values[i] = ai * values[i-1] + values[i-2];
            }
        }
        values_len = n + 1;   /* we have stored indices 0 … n */
    }

    return values[n];
}
