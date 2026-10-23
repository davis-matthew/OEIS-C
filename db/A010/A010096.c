/*
 * A010096  log2*(n) (version 1): number of times floor(log_2(x)) is used in
 *          floor(log_2(floor(log_2(...(floor(log_2(n)))...)))) = 0.
 * https://oeis.org/A010096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of iterations of floor(log2(x)) needed to reach 0,
 *            starting from x = n (n ≥ 1)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes log₂∗(n) – the number of times one must apply
   floor(log2) to n until 0 is produced.  The function works
   for any 64‑bit n; the result easily fits in a seq_t. */
#include <stdint.h>   /* for idx_t typedefs used by the build */

seq_t a(idx_t n)
{
    seq_t count = 0;        /* number of iterations performed */
    idx_t x = n;            /* current value */

    while (x > 0) {
        /* floor(log2(x)) is the position of the most significant
           set bit.  Using the GCC/Clang builtin gives O(1) time.  */
        x = 63 - __builtin_clzll(x);
        count++;
    }

    return count;
}
