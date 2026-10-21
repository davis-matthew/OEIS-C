/*
 * A200462  Number of 0..1 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its four previous neighbors modulo 2.
 * https://oeis.org/A200462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed from a 4–state automaton describing the
 *            constraint that each element must be at least the sum of its
 *            four previous elements modulo 2. No simple closed form is used
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 66, u64 n <= 143,
 *            u128 n <= 210, big any n
 * Helpers:   matrix_mul, matrix_power, vector_mul_matrix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/matrix_mul.h"
#include "../utils/matrix_power.h"
#include "../utils/vector_mul_matrix.h"

static seq_t M[16][16];
static seq_t init_vec[16];
static bool built = false;

static void build_automaton(void) {
    if (built) return;
    built = true;

    /* zero the transition matrix */
    for (idx_t i = 0; i < 16; ++i)
        for (idx_t j = 0; j < 16; ++j)
            M[i][j] = 0;

    /* build transitions */
    for (idx_t s = 0; s < 16; ++s) {
        int parity = __builtin_parity((unsigned)s); /* parity of 4 bits */
        idx_t t;
        if (parity) {          /* forced new bit = 1 */
            t = ((s << 1) & 15) | 1;
            M[s][t] = 1;
        } else { /* free bit: 0 or 1 */
            t = ((s << 1) & 15);
            M[s][t] = 1;
            M[s][t | 1] = 1;
        }
    }

    /* initial vectors for prefixes of length 4 that satisfy internal constraints */
    for (idx_t s = 0; s < 16; ++s) {
        int b0 = (s >> 3) & 1; /* x0   */
        int b1 = (s >> 2) & 1; /* x1   */
        int b2 = (s >> 1) & 1; /* x2   */
        int b3 = s & 1;        /* x3   */
        if ((b1 >= b0) &&
            (b2 >= ((b0 + b1) & 1)) &&
            (b3 >= ((b0 + b1 + b2) & 1))) {
            init_vec[s] = 1;
        } else {
            init_vec[s] = 0;
        }
    }
}

seq_t a(idx_t n) {
    /* handle small n explicitly */
    if (n == 0) return 0;            /* not defined but harmless */
    if (n == 1) return 2;
    if (n == 2) return 3;
    if (n == 3) return 5;
    if (n < 4) return 0; /* unreachable but keeps compiler happy */

    build_automaton();

    idx_t steps = n - 4;                 /* number of transitions needed */
    seq_t power[16][16];
    matrix_power(M, steps, power);

    seq_t cur[16];
    vector_mul_matrix(init_vec, power, cur);

    seq_t total = 0;
    for (idx_t i = 0; i < 16; ++i)
        total += cur[i];
    return total;
}
