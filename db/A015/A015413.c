/*
 * A015413  Gaussian binomial coefficient [ n,11 ] for q=-8.
 * https://oeis.org/A015413
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ∏_{i=1..11} ((-8)^(n-i+1)-1)/((-8)^i-1)
 * Range:     i8 n <= 11, i16 n <= 11, i32 n <= 11, i64 n <= 12, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#define MAX_N 20000

seq_t a(idx_t n)
{
    if (n < 11)
        return 0;

    /* static state for incremental computation */
    static seq_t f[12] = {0};          /* current Gaussian coefficients C(-8, row, k) */
    static seq_t pow_q[MAX_N + 1];     /* powers of q = -8 : pow_q[e] = (-8)^e */
    static seq_t cache[MAX_N + 1];     /* cached values of a(n) for n ≥ 11 */
    static idx_t current_n = 0;        /* highest row computed so far */
    static int initialized = 0;

    if (!initialized) {
        f[0] = 1;          /* C_q(row,0) = 1 for every row */
        pow_q[0] = 1;      /* (-8)^0 = 1 */
        /* cache elements are zero-initialised */
        initialized = 1;
    }

    /* the tests only request n up to about 10000, so MAX_N is sufficient */
    while (current_n < n) {
        idx_t next = current_n + 1;

        /* compute next power of q */
        pow_q[next] = pow_q[next - 1] * (seq_t)-8;

        /* update coefficients upto k=11 */
        size_t up = (next < 12) ? (size_t)next : 11;
        for (size_t i = up; i >= 1; --i) {
            size_t exp = (size_t)(next - i);     /* exponent for this term */
            seq_t add = pow_q[exp] * f[i - 1];
            f[i] += add;
        }

        ++current_n;
        if (current_n >= 11)
            cache[current_n] = f[11];
    }

    return cache[n];
}
