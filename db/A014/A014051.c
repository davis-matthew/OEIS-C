/*
 * A014051  Inverse of 42nd cyclotomic polynomial.
 * https://oeis.org/A014051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the nth coefficient of 1/Φ₄₂(x) (the inverse of the 42nd
 *            cyclotomic polynomial). The sequence has period 42 and satisfies
 *            the linear recurrence a(n) + a(n-1) - a(n-3) - a(n-4) + a(n-6) -
 *            a(n-8) - a(n-9) + a(n-11) + a(n-12) = 0 with initial terms
 *            a(0)=1, a(1)=-1, a(2)=1, a(3)=0, … (as list...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t coeff[13] = {1,1,0,-1,-1,0,1,0,-1,-1,0,1,1};
static seq_t table[42];
static int ready = 0;

static void init_table(void) {
    if (ready) return;
    table[0] = 1;
    for (idx_t i = 1; i < 42; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 12; ++k) {
            if (i >= (idx_t)k) {
                sum += coeff[k] * table[i - k];
            }
        }
        table[i] = -sum;
    }
    ready = 1;
}

seq_t a(idx_t n) {
    init_table();
    return table[(size_t)(n % 42)];
}
