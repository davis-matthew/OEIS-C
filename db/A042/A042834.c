/*
 * A042834  Numerators of continued fraction convergents to sqrt(948).
 * https://oeis.org/A042834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = Numerators of continued fraction convergents to sqrt(948)
 *            (recurrence: p_-2 = 0, p_-1 = 1, p_k = a_k * p_{k-1} + p_{k-2},
 *            with a_0 = floor(sqrt(948)) = 30 and a_k for k>=1 given by the
 *            periodic continued‑fraction partial quotients of sqrt(948).)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper to compute the period of the continued fraction for sqrt(948).
   It is stored in static arrays so subsequent calls reuse the result. */
static void init_period(seq_t **period_ptr, size_t *period_len_ptr)
{
    static int initialized = 0;
    if (initialized)
        return;
    const uint64_t N = 948;
    const uint64_t a0 = 30;          /* floor(sqrt(948)) */

    /* Temporary storage during construction */
    seq_t *parts = (seq_t *)malloc(64 * sizeof(seq_t));
    size_t len = 0;
    int64_t m = 0;
    int64_t d = 1;
    int64_t a = a0;

    /* Generate partial quotients until the term 2*a0 appears,
       which marks the end of the period for sqrt(N). */
    do {
        m = d * a - m;
        d = (N - m * m) / d;
        a = (a0 + m) / d;
        parts[len++] = (seq_t)a;
        if (len == 64) {                /* grow if necessary */
            parts = (seq_t *)realloc(parts, (len + 64) * sizeof(seq_t));
        }
    } while (a != 2 * a0);

    *period_ptr = parts;
    *period_len_ptr = len;
    initialized = 1;
}

seq_t a(idx_t n)
{
    /* Periodic partial quotients for sqrt(948) */
    static seq_t *period = NULL;
    static size_t period_len = 0;
    if (!period) {
        init_period(&period, &period_len);
    }

    const seq_t a0 = (seq_t)30;          /* first partial quotient */

    seq_t p_nm2 = 0;     /* p_{-2} */
    seq_t p_nm1 = 1;     /* p_{-1} */
    seq_t p_n = 0;

    /* Iterate the recurrence up to index n */
    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0) {
            p_n = a0;
        } else {
            seq_t ak = period[(size_t)((i - 1) % period_len)];
            p_n = ak * p_nm1 + p_nm2;
        }
        if (i == n)
            return p_n;
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }
    /* In practice we never reach here */
    return 0;
}
