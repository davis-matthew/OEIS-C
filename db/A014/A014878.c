/*
 * A014878  Inverse of 869th cyclotomic polynomial.
 * https://oeis.org/A014878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A014878: Inverse of the 869th cyclotomic polynomial
 *
 * The coefficients satisfy the linear recurrence
 *   a(n) = a(n-1) + a(n-869) – a(n-870) + rhs(n)
 * for n ≥ 0 with a(k) = 0 for k < 0, where
 *   rhs(0) =  1,
 *   rhs(11)= -1,   rhs(79)= -1,   rhs(90)= +1,
 *   rhs(n) = 0 otherwise.
 * The first few terms are
 *   a(0)=1, a(1)=1,…,a(10)=1, a(11)=0,…,a(23)=0.
 *
 * This implementation evaluates the recurrence directly,
 * using a circular buffer of length 871 (to store the
 * last 871 values).  It works for any seq_t that supports
 * the standard arithmetic operators, and the code is
 * valid C11 and C++17.
 */
#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    /* circular buffer to keep the last 871 computed values */
    static seq_t buf[871];
    /* zero out the buffer immediately so that older values do not
     * leak between calls for small n.  Only required when the
     * buffer might contain garbage from a previous larger call.
     * This is harmless because we overwrite all indices used
     * before accessing them.
     */
    for (size_t i = 0; i < 871; ++i)
        buf[i] = (seq_t)0;

    /* compute a(0) .. a(n) sequentially */
    for (idx_t i = 0; i <= n; ++i) {
        /* a(i-1) */
        seq_t term_prev = (i >= 1) ? buf[(i - 1) % 871] : (seq_t)0;
        /* a(i-869) */
        seq_t term_869 = (i >= 869) ? buf[(i - 869) % 871] : (seq_t)0;
        /* a(i-870) */
        seq_t term_870 = (i >= 870) ? buf[(i - 870) % 871] : (seq_t)0;

        /* left side of the recurrence */
        seq_t left = term_prev + term_869 - term_870;

        /* right‑hand side coefficients for the polynomial */
        seq_t rhs = (seq_t)0;
        if (i == 0)          rhs = (seq_t)1;
        else if (i == 11)    rhs = (seq_t)-1;
        else if (i == 79)    rhs = (seq_t)-1;
        else if (i == 90)    rhs = (seq_t)1;

        /* store the computed value */
        buf[i % 871] = left + rhs;
    }

    return buf[n % 871];
}
