/*
 * A174995  Products of decimal digits.
 * https://oeis.org/A174995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is the nth smallest unique product of two decimal digits
 *            (i.e., the nth element of the sorted set {i*j | 0≤i,j≤9}). The
 *            sequence has 37 terms and stops at 81
 * Range:     u8 n <= 37, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool initialized = false;
static seq_t values[37];
static idx_t count = 0;

seq_t a(idx_t n) {
    if (!initialized) {
        /* compute all products i*j, 0<=i,j<=9, and collect unique values */
        bool present[82] = {false};  /* 0..81 inclusive */
        for (int i = 0; i <= 9; ++i) {
            for (int j = 0; j <= 9; ++j) {
                present[i * j] = true;
            }
        }
        seq_t *p = values;
        for (int v = 0; v <= 81; ++v)
            if (present[v]) *p++ = (seq_t)v;
        count = p - values;
        initialized = true;
    }
    if (n == 0 || n > count) return (seq_t)0;
    return values[n - 1];
}
