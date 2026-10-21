/*
 * A091877  Square roots of A068178.
 * https://oeis.org/A091877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=2, a(2)=7, a(3)=43. For n≥4, a(n) is the decimal number
 *            that starts with a '1', followed by (n‑3) copies of the digit
 *            '3', and ends with a '6'. Equivalently, a(n) = (4·10^(n‑1) +
 *            8)/3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n) {
    /* Handle the first three terms explicitly */
    if (n == (idx_t)1) return (seq_t)2;
    if (n == (idx_t)2) return (seq_t)7;
    if (n == (idx_t)3) return (seq_t)43;

    /* For n ≥ 4: build the number digit by digit.
       value = 1; for each of the (n-3) 3's: value = value*10 + 3
       finally append the final 6: value = value*10 + 6. */
    seq_t value = (seq_t)1;
    idx_t i;
    for (i = (idx_t)0; i < n - (idx_t)3; ++i) {
        value = value * (seq_t)10 + (seq_t)3;
    }
    value = value * (seq_t)10 + (seq_t)6;
    return value;
}
