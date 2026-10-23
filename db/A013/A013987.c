/*
 * A013987  Expansion of 1/(1-x^2-x^3-x^4-x^5-x^6-x^7-x^8-x^9-x^10).
 * https://oeis.org/A013987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 49, u64 n <= 95, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the OEIS sequence A013987: number of compositions of n
 * into parts 2 <= p <= 10.
 *
 * Recurrence:
 *   a(0) = 1, a(1) = 0
 *   a(n) = a(n-2) + a(n-3) + a(n-4) + a(n-5) + a(n-6) +
 *          a(n-7) + a(n-8) + a(n-9) + a(n-10)   for n >= 2
 *
 * The function returns the exact value of a(n) in the type seq_t.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the two base cases */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)0;
    }

    /* Allocate array of size n+1 to hold all computed values */
    seq_t *val = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!val) {
        /* Out of memory; return 0 to indicate failure */
        return (seq_t)0;
    }

    /* Initialise the first two terms */
    val[0] = (seq_t)1;
    val[1] = (seq_t)0;

    /* Compute values iteratively */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        if (i >= 2) sum += val[i - 2];
        if (i >= 3) sum += val[i - 3];
        if (i >= 4) sum += val[i - 4];
        if (i >= 5) sum += val[i - 5];
        if (i >= 6) sum += val[i - 6];
        if (i >= 7) sum += val[i - 7];
        if (i >= 8) sum += val[i - 8];
        if (i >= 9) sum += val[i - 9];
        if (i >= 10) sum += val[i - 10];
        val[i] = sum;
    }

    seq_t result = val[n];
    free(val);
    return result;
}
