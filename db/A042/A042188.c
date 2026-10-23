/*
 * A042188  Numerators of continued fraction convergents to sqrt(619).
 * https://oeis.org/A042188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the nth convergent of √619 (index 0‑based)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 18, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *num = NULL;     /* cache of numerators */
static size_t cap = 0, len = 0;   /* current capacity and number of cached terms */

static seq_t p_prevprev, p_prev; /* previous two numerators */
static seq_t m, d, a_cur;       /* state of the continued fraction */
static const seq_t a0 = 24;            /* floor(sqrt(619)) */
static const seq_t D  = 619;           /* the radicand */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cap) return;
    size_t newcap = cap ? cap << 1 : 16;
    if (newcap < need) newcap = need;
    seq_t *tmp = (seq_t*)realloc(num, newcap * sizeof(seq_t));
    if (!tmp) abort(); /* out of memory; allow abort for simplicity */
    num = tmp;
    cap = newcap;
}

/* Return the nth numerator of the convergents to √619. */
seq_t a(idx_t n)
{
    /* Lazy initialization. */
    if (len == 0) {
        p_prevprev = 0; /* p_{-1} */
        p_prev     = 1; /* p_0 will be computed next */
        m = 0;
        d = 1;
        a_cur = a0;

        /* Compute term 0: p_1 = a0 * p_0 + p_{-1} = a0 * 1 + 0 = a0. */
        seq_t p0 = a_cur * p_prev + p_prevprev;

        ensure_capacity(1);
        num[0] = p0;

        /* Prepare state for the next term. */
        seq_t new_m = d * a_cur - m;
        seq_t new_d = (D - new_m * new_m) / d;
        seq_t new_a = (a0 + new_m) / new_d;
        m = new_m; d = new_d; a_cur = new_a;

        /* Update bookkeeping. */
        p_prevprev = p_prev;   /* now p_0 */
        p_prev = p0;           /* now p_1 */
        len = 1;               /* one term cached */
    }

    /* Extend the cache up to the requested index. */
    while (len <= n) {
        seq_t a_k = a_cur;                   /* current continued fraction term */
        seq_t p_k = a_k * p_prev + p_prevprev;  /* next numerator */

        ensure_capacity(len + 1);
        num[len] = p_k;

        /* Shift numerators for next iteration. */
        p_prevprev = p_prev;
        p_prev = p_k;

        /* Compute the next continued fraction term. */
        seq_t new_m = d * a_cur - m;
        seq_t new_d = (D - new_m * new_m) / d;
        seq_t new_a = (a0 + new_m) / new_d;
        m = new_m; d = new_d; a_cur = new_a;

        ++len;
    }

    return num[n];
}
