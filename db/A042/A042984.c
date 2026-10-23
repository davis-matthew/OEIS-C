/*
 * A042984  Number of n-dimensional partitions of 6.
 * https://oeis.org/A042984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+4)*(n^3 + 40*n^2 + 61*n + 30)/120
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 211, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t for calculations */
    seq_t a_val = (seq_t)n + (seq_t)1;      /* n+1  */
    seq_t b_val = (seq_t)n + (seq_t)4;      /* n+4  */
    seq_t n_sq   = (seq_t)n * (seq_t)n;     /* n^2  */
    seq_t n_cu   = n_sq * (seq_t)n;         /* n^3  */

    /* c = n^3 + 40*n^2 + 61*n + 30 */
    seq_t c_val = n_cu;
    c_val += (seq_t)40 * n_sq;
    c_val += (seq_t)61 * (seq_t)n;
    c_val += (seq_t)30;

    /* Cancel the factor 120 = 2^3 * 3 * 5 among the three factors */
    static const int primes[5] = {5, 3, 2, 2, 2};
    int p = 0;
    for (p = 0; p < 5; ++p) {
        int pr = primes[p];
        if (a_val % (seq_t)pr == (seq_t)0) {
            a_val /= (seq_t)pr;
            continue;
        }
        if (b_val % (seq_t)pr == (seq_t)0) {
            b_val /= (seq_t)pr;
            continue;
        }
        /* c_val must be divisible by the remaining prime */
        if (c_val % (seq_t)pr == (seq_t)0) {
            c_val /= (seq_t)pr;
            continue;
        }
        /* Should never happen for correct formula */
    }

    /* Now (a_val * b_val * c_val) is the desired term */
    seq_t result = a_val * b_val;
    result *= c_val;

    return result;
}
