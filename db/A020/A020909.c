/*
 * A020909  Number of bits in the base-2 representation of the n-th Fibonacci
 *          number.
 * https://oeis.org/A020909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 1 + floor(log_2(F(n)))
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

/* Compute the bit length of a non‑negative seq_t value.
   It returns the number of bits required to represent the value
   (the position of the most significant 1‑bit plus one).  */
static inline seq_t bitlen(seq_t x) {
    seq_t cnt = 0;
    while (x) {
        x >>= 1;
        cnt++;
    }
    return cnt;
}

seq_t a(idx_t n) {
    /* a(1) = 1 as required by the OEIS offset (index starts at 1). */
    if (n == 0)
        return 0;   /* undefined for n < 1, but harmless here */

    seq_t fn = fib(n);
    return bitlen(fn);
}
