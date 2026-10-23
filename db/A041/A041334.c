/*
 * A041334  Numerators of continued fraction convergents to sqrt(181).
 * https://oeis.org/A041334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 13, a(1) = 27 a(n) = coeff(n‑1) * a(n‑1) + a(n‑2) for n ≥
 *            2 coeff(k) are the partial quotients (after the initial a₀) of
 *            the continued‑fraction expansion of √181
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 41, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The continued fraction of √181 has a purely periodic part after
   the initial term a₀ = ⌊√181⌋ = 13.  Its period is computed once
   and stored in an array of unsigned long long integers.  The
   sequence a(n) is then obtained by the standard recurrence for
   convergents.  No floating‑point operations are used; the code
   works for any seq_t that supports +, −, * on integers.  */

#include <stdlib.h>
#include <stdint.h>

/* --- Compute the period of the continued fraction of √181 once --- */

static void compute_period(unsigned long long **period, size_t *len)
{
    const unsigned long long N  = 181ull;
    const unsigned long long a0 = 13ull;    /* floor(√181) */
    unsigned long long m = 0, d = 1, a = a0;
    size_t capacity = 16, i = 0;

    *period = (unsigned long long *)malloc(capacity * sizeof(unsigned long long));
    *len = 0;

    do {
        m = d * a - m;
        d = (N - m * m) / d;
        a = (a0 + m) / d;

        if (i == capacity) {
            capacity *= 2;
            *period = (unsigned long long *)realloc(*period,
                                                    capacity * sizeof(unsigned long long));
        }
        (*period)[i++] = a;
    } while (a != 2 * a0);

    *len = i;
}

/* --- Public sequence function --- */

seq_t a(idx_t n)
{
    /* static data for the period of the continued fraction */
    static unsigned long long *period = NULL;
    static size_t period_len = 0;
    static int period_ready = 0;

    if (!period_ready) {
        compute_period(&period, &period_len);
        period_ready = 1;
    }

    /* Special case n = 0 */
    if (n == 0) {
        return (seq_t)13;
    }

    /* Recurrence: p_-1 = 1, p_0 = 13.
       For n ≥ 1: p_n = coeff(n-1) * p_{n-1} + p_{n-2}. */
    seq_t p_prevprev = (seq_t)1;   /* p_{-1} */
    seq_t p_prev     = (seq_t)13;  /* p_0 */
    seq_t p_curr;

    idx_t i;
    for (i = 1; i <= n; ++i) {
        unsigned long long coeff = period[(i - 1) % period_len];
        p_curr = (seq_t)coeff * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev = p_curr;
    }

    return p_prev;
}
