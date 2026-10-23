/*
 * A018311  Divisors of 162.
 * https://oeis.org/A018311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 162 in ascending order */
#include <stdbool.h>

seq_t a(idx_t n) {
    /* cache the 10 divisors of 162: 1,2,3,6,9,18,27,54,81,162 */
    static seq_t divisors[10];
    static bool initialized = false;
    if (!initialized) {
        /* 3^0..3^4 */
        seq_t pow3[5] = {(seq_t)1, (seq_t)3, (seq_t)9, (seq_t)27, (seq_t)81};
        /* 2^0..2^1 */
        seq_t pow2[2] = {(seq_t)1, (seq_t)2};
        int k = 0;
        for (int e3 = 0; e3 < 5; ++e3) {
            seq_t d3 = pow3[e3];
            for (int e2 = 0; e2 < 2; ++e2) {
                divisors[k++] = d3 * pow2[e2];
            }
        }
        initialized = true;
    }
    if (n == 0) return (seq_t)0;          /* n < 1 is undefined */
    if (n <= 10) return divisors[n - 1];   /* valid terms */
    return (seq_t)0;                       /* sequence defined only up to 10 terms */
}
