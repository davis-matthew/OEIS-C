/*
 * A208060  a(n) = 1 + 2*n + 2^2*n*[n/2] + 2^3*n*[n/2]*[n/3] +
 *          2^4*n*[n/2]*[n/3]*[n/4] + ... where [x]=floor(x).
 * https://oeis.org/A208060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS): a(n) = 1 + 2*n + 2^2*n*⌊n/2⌋ + 2^3*n*⌊n/2⌋*⌊n/3⌋ + ... */
/* Compute by recurrence: term_m = previous_term * 2 * ⌊n/m⌋ for m≥2   */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* initial result 1 + first term 2*n */
    seq_t result = 1;
    seq_t term   = (seq_t)2 * (seq_t)n;     /* m = 1 */
    result += term;

    /* subsequent terms */
    for (idx_t m = 2; m <= n; ++m) {
        seq_t f = (seq_t)(n / m);          /* floor(n/m) */
        /* For m > n, f becomes 0 and the loop would terminate naturally */
        term = term * (seq_t)2 * f;
        result += term;
    }

    return result;
}
