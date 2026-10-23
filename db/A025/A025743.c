/*
 * A025743  Index of 10^n within the sequence of the numbers of the form
 *          5^i*10^j.
 * https://oeis.org/A025743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=0..n} ceil(k * c), c = log_5(10)
 * Range:     u8 n <= 18, u16 n <= 301, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Rational approximation of log5(2) = ln(2)/ln(5) :
   log5(2) ≈ 0.430676558073245171 ...
   We use p/q with p = 430676558073245, q = 10^15,
   giving an absolute error < 2·10^-15.  */
static const unsigned long long LOG5_2_P = 430676558073245ULL; /* numerator   */
static const unsigned long long LOG5_2_Q = 1000000000000000ULL; /* denominator */

seq_t a(idx_t n)
{
    /* Sum floor(k*log5(2)) for k = 1..n */
    seq_t sum_floor = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* floor(k * p / q) computed with integer arithmetic */
        seq_t floor_log = (seq_t)(((__int128)k * LOG5_2_P) / LOG5_2_Q);
        sum_floor += floor_log;
    }

    /* a(n) = 1 + n(n+3)/2 + sum_floor */
    seq_t term = 1;
    seq_t n_seq = (seq_t)n;
    term += (seq_t)(((__int128)n_seq * (n_seq + 3)) / 2);
    term += sum_floor;

    return term;
}
