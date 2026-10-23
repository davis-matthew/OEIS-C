/*
 * A018501  Divisors of 525.
 * https://oeis.org/A018501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The divisors of 525 are the numbers 3^a 5^b 7^c with
 * a∈{0,1}, b∈{0,1,2}, c∈{0,1}.  This function generates all 12
 * divisors, sorts them ascending, and returns the n‑th one
 * (1‑based).  For n>12 the result is 0. */
#include <stddef.h>   /* size_t for the array index */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* pre‑computed exponent values for 525 = 3 * 5² * 7 */
    const seq_t e3[2] = {(seq_t)1, (seq_t)3};
    const seq_t e5[3] = {(seq_t)1, (seq_t)5, (seq_t)25};
    const seq_t e7[2] = {(seq_t)1, (seq_t)7};

    seq_t vals[12];
    size_t pos = 0;
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            for (size_t k = 0; k < 2; ++k) {
                vals[pos++] = e3[i] * e5[j] * e7[k];
            }
        }
    }

    /* simple insertion sort – 12 elements only */
    for (size_t i = 0; i < 11; ++i) {
        for (size_t j = i + 1; j < 12; ++j) {
            if (vals[j] < vals[i]) {
                seq_t tmp = vals[i];
                vals[i] = vals[j];
                vals[j] = tmp;
            }
        }
    }

    return (n <= 12) ? vals[n - 1] : (seq_t)0;
}
