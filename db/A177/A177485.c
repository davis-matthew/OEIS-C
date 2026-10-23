/*
 * A177485  G.f.: (1+x+x^3+x^5)/( (1-x^2+x^3)*(1-x-x^3) ).
 * https://oeis.org/A177485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + x + x³ + x⁵) / ((1 - x² + x³)(1 - x - x³))
 *            Recurrence (for n ≥ 6): a(n) = a(n‑1) + a(n‑2) – a(n‑3) + a(n‑4)
 *            – a(n‑5) + a(n‑6) Initial values: a(0)=1, a(1)=2, a(2)=3,
 *            a(3)=5, a(4)=7, a(5)=11
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* for size_t */
#include <stdlib.h>    /* for malloc, realloc, exit */

static const seq_t init_vals[6] = {1, 2, 3, 5, 7, 11};

seq_t a(idx_t n)
{
    if (n < 6) return init_vals[n];

    /* Keep the last six values of the sequence in a circular buffer.
       v[0] … v[5] correspond to a[n‑6] … a[n‑1] during the loop. */
    seq_t v[6];
    for (idx_t i = 0; i < 6; ++i) v[i] = init_vals[i];

    seq_t next;
    for (idx_t i = 6; i <= n; ++i) {
        /* Compute a(i) = a(i‑1)+a(i‑2)−a(i‑3)+a(i‑4)−a(i‑5)+a(i‑6) */
        next = v[5] + v[4] - v[3] + v[2] - v[1] + v[0];
        /* Shift the buffer: drop a(i‑6) and slide toward older indices. */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }

    return v[5];   /* v[5] now holds a(n) */
}
