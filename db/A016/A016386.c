/*
 * A016386  340th cyclotomic polynomial.
 * https://oeis.org/A016386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 340th cyclotomic polynomial
 *            Φ_340(x); computed via the product formula Φ_n(x)=∏_{d|340}
 *            (x^d−1)^{μ(340/d)}
 * Range:     i8 n <= 128, i16 n <= 128, i32 n <= 128, i64 n <= 128,
 *            i128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_POLY 400   /* maximum degree needed during intermediate steps */

/* local helper: multiply current polynomial by (x^d-1). */
static void mult_xd_minus_1(seq_t *poly, int *size, int d)
{
    int old_size = *size;
    int new_size = old_size + d;
    seq_t newpoly[MAX_POLY];
    for (int i = 0; i < new_size; ++i) newpoly[i] = 0;

    for (int i = 0; i < old_size; ++i) {
        newpoly[i + d] += poly[i];
        newpoly[i]     -= poly[i];
    }
    for (int i = 0; i < new_size; ++i) poly[i] = newpoly[i];
    *size = new_size;
}

/* local helper: divide polynomial by (x^d-1). Assumes divisor is monic. */
static void div_xd_minus_1(const seq_t *poly, int size,
                           seq_t *quot, int *quot_size, int d)
{
    int qsize = size - d;
    seq_t r[MAX_POLY];
    for (int i = 0; i < size; ++i) r[i] = poly[i];
    for (int i = 0; i < qsize; ++i) quot[i] = 0;

    for (int i = size - 1; i >= d; --i) {
        seq_t factor = r[i];
        if (factor) {
            r[i] = 0;
            r[i - d] += factor;
            quot[i - d] += factor;
        }
    }
    *quot_size = qsize;
}

seq_t a(idx_t n)
{
    static int init = 0;
    static seq_t coeff[129];
    if (!init) {
        init = 1;
        /* arrays for the intermediate polynomial */
        seq_t buf[MAX_POLY] = {0}, next[MAX_POLY] = {0};
        int cur_size = 1;
        buf[0] = 1;   /* start with polynomial 1 */

        /* + divisors where μ(340/d) = +1 */
        const int plus[] = {4, 10, 34, 340};
        /* - divisors where μ(340/d) = –1 */
        const int minus[] = {2, 20, 68, 170};

        /* multiply by each (x^d-1) where exponent is +1 */
        for (int i = 0; i < 4; ++i)
            mult_xd_minus_1(buf, &cur_size, plus[i]);

        /* divide by each (x^d-1) where exponent is –1 */
        for (int i = 0; i < 4; ++i) {
            int new_size;
            div_xd_minus_1(buf, cur_size, next, &new_size, minus[i]);
            for (int j = 0; j < new_size; ++j) buf[j] = next[j];
            cur_size = new_size;
        }

        /* cur_size should now be 129 (degree φ(340)=128 + 1). */
        for (int i = 0; i < 129; ++i)
            coeff[i] = buf[i];
    }

    if (n < 129)
        return coeff[(int)n];
    else
        return 0;   /* indices beyond the polynomial yield 0 */
}
