/*
 * A051211  Numbers of the form x^2-9^y >= 0.
 * https://oeis.org/A051211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers of the form x^2 - 9^y >= 0 (sorted, first term is 0)
 * Range:     u8 n <= 39, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* YMAX is the maximal exponent y that can contribute to the first few thousand terms.
 * For n up to 10 000 the term a(n) is below a few millions, so exponents up to 12
 * are more than enough (3^12 = 531441). */
#define YMAX 12

seq_t a(idx_t n)
{
    /* Precompute 3^y and 9^y in wider integer types to avoid overflow. */
    idx_t pow3[YMAX + 1];
    unsigned long long pow9[YMAX + 1];
    pow3[0] = 1U;
    pow9[0] = 1ULL;
    for (idx_t i = 1; i <= YMAX; ++i) {
        pow3[i] = pow3[i - 1] * 3U;
        pow9[i] = pow9[i - 1] * 9ULL;
    }

    /* Current x for each exponent y and the current value x^2 - 9^y. */
    idx_t curr_x[YMAX + 1];
    unsigned long long val[YMAX + 1];
    for (idx_t i = 0; i <= YMAX; ++i) {
        curr_x[i] = pow3[i];                       /* x starts at 3^y    */
        unsigned long long xsq = (unsigned long long)curr_x[i] *
                                (unsigned long long)curr_x[i];
        val[i] = xsq - pow9[i];                     /* initial value = 0  */
    }

    idx_t count = 0;                                 /* how many unique terms found */
    unsigned long long last_val = 0ULL;              /* last value emitted     */
    bool has_last = false;                           /* flag for first value   */
    seq_t result = 0;                                /* value to return        */

    while (true) {
        /* Find sequence with minimal current value. */
        idx_t min_i = 0;
        unsigned long long min_val = val[0];
        for (idx_t i = 1; i <= YMAX; ++i) {
            if (val[i] < min_val) {
                min_val = val[i];
                min_i = i;
            }
        }

        /* Emit the value if it is not a duplicate of the previous one. */
        if (!has_last || min_val != last_val) {
            ++count;
            if (count == n) {
                result = (seq_t)min_val;
                break;
            }
            last_val = min_val;
            has_last = true;
        }

        /* Advance the sequence that produced the current minimal value. */
        curr_x[min_i]++;                                   /* next x   */
        unsigned long long xsq = (unsigned long long)curr_x[min_i] *
                                 (unsigned long long)curr_x[min_i];
        val[min_i] = xsq - pow9[min_i];
    }

    return result;
}
