/*
 * A088002  Expansion of (1+x^2)/(1+x^2+x^5).
 * https://oeis.org/A088002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in (1+x^2)/(1+x^2+x^5) Recurrence:
 *            a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(n) = -a(n-2) - a(n-5)
 *            for n ≥ 5
 * Range:     i8 n <= 38, i16 n <= 70, i32 n <= 133, i64 n <= 262,
 *            i128 n <= 523, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define A088002_MAX_N 20000

static seq_t seq[A088002_MAX_N + 1];
static idx_t seq_computed_max = (idx_t)-1;  /* uninitialized */

seq_t a(idx_t n) {
    if (seq_computed_max == (idx_t)-1) {
        /* Base values */
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)0;
        seq[2] = (seq_t)0;
        seq[3] = (seq_t)0;
        seq[4] = (seq_t)0;
        seq_computed_max = 4;
    }
    if (n > seq_computed_max && n <= A088002_MAX_N) {
        for (idx_t i = seq_computed_max + 1; i <= n; ++i) {
            seq[i] = -(seq[i-2]) - (seq[i-5]);
        }
        seq_computed_max = n;
    }
    return seq[n];
}
