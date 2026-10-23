/*
 * A107221  Numbers such that sum of digits is 7 and product of digits is 8.
 * https://oeis.org/A107221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   numbers whose decimal digits sum to 7 and whose digits product
 *            to 8
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is finite; the first 10 terms are: 124, 142, 214, 241, 412, 421,
   1222, 2122, 2212, 2221. */
#include <stdlib.h>

static seq_t a107221_values[10];
static int a107221_count = 0;
static int a107221_initialized = 0;

static void init_a107221(void) {
    if (a107221_initialized) return;
    for (int num = 1; num < 10000; ++num) {
        int x = num;
        int sum = 0;
        int prod = 1;
        while (x > 0) {
            int d = x % 10;
            sum += d;
            prod *= d;
            x /= 10;
        }
        if (sum == 7 && prod == 8) {
            a107221_values[a107221_count++] = (seq_t)num;
        }
    }
    a107221_initialized = 1;
}

seq_t a(idx_t n) {
    init_a107221();          /* generate the list once */
    if (n == 0) return (seq_t)0;   /* below offset: undefined */
    if (n <= (idx_t)a107221_count)
        return a107221_values[n - 1];
    return (seq_t)0;           /* no further terms */
}
