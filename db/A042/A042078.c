/*
 * A042078  Numerators of continued fraction convergents to sqrt(563).
 * https://oeis.org/A042078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042078
   Numerators of continued‑fraction convergents to sqrt(563).
   a(0) = 23.
   For n ≥ 1, let q(n) be the n‑th partial quotient of the simple continued
   fraction of sqrt(563); then
          a(n) = q(n) * a(n‑1) + a(n‑2).
   The partial quotients after the initial term are periodic with period
   length 10 (q(1)…q(10)), which the routine computes once. */

#include <stdlib.h>

static const int D = 563;        /* the square‑free integer */
static const int a0 = 23;        /* floor(sqrt(563)) */

/* Periodic part of the continued fraction for sqrt(563). */
static int period_coeff[128];
static int period_len = 0;

/* Storage for numerators */
static seq_t *num_arr   = NULL;
static size_t num_cap   = 0;   /* allocated capacity in elements */
static idx_t num_count  = 0;   /* number of elements actually stored */

/* Recurrence state */
static seq_t p_prevprev = 0;   /* corresponds to a_{-2} */
static seq_t p_prev     = 1;   /* corresponds to a_{-1} */

/* Compute the periodic part of the continued fraction once. */
static void init_period(void) {
    int m = 0, d = 1, a = a0;
    int i = 0;
    while (1) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period_coeff[i++] = a;
        if (a == 2 * a0) break;      /* end of period */
        if (i >= 127) break;         /* safety guard */
    }
    period_len = i;
}

/* Ensure the numerator array has room for indices up to 'n'. */
static void ensure_num_capacity(idx_t n) {
    if (n + 1 <= num_cap) return;
    size_t new_cap = num_cap ? num_cap * 2 : 64;
    while ((idx_t)new_cap <= n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(num_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failed */
    num_arr = tmp;
    num_cap = new_cap;
}

/* Main sequence routine. */
seq_t a(idx_t n) {
    if (period_len == 0) init_period();   /* compute period once */

    if (n >= num_count) {                /* compute additional terms if needed */
        ensure_num_capacity(n);
        for (idx_t i = num_count; i <= n; ++i) {
            int coeff = (i == 0) ? a0 : period_coeff[(i - 1) % period_len];
            seq_t val = (seq_t)coeff * p_prev + p_prevprev;
            num_arr[i] = val;
            /* advance recurrence state */
            p_prevprev = p_prev;
            p_prev = val;
        }
        num_count = n + 1;
    }

    return num_arr[n];
}
