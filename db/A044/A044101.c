/*
 * A044101  Numbers k such that string 4,4 occurs in the base 5 representation
 *          of k but not of k-1.
 * https://oeis.org/A044101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by scanning for numbers whose base-5 representation contains
   the contiguous digits "44" but the preceding integer does not. */

#include <stdbool.h>
#include <stdint.h>

/* Helper: true iff x (in base 5) contains the substring "44". */
static bool has44(seq_t x) {
    int prevdigit = -1;               /* previous LSB, -1 means none */
    while (x > (seq_t)0) {
        int digit = (int)(x % (seq_t)5);
        if (prevdigit == 4 && digit == 4)
            return true;              /* pattern found */
        prevdigit = digit;
        x /= (seq_t)5;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = (seq_t)1;               /* start from 1 */
    while (1) {
        if (has44(k) && !has44(k - (seq_t)1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
