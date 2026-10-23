/*
 * A118610  Start with 1 and repeatedly reverse the digits and add 24 to get
 *          the next term.
 * https://oeis.org/A118610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   start with 1 and repeatedly reverse decimal digits and add 24
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Reverse the decimal digits of x (works for all supported seq_t types) */
static seq_t rev_dec(seq_t x) {
    seq_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x = x / 10;
    }
    return r;
}

/* Compute the n-th term of OEIS A118610 */
seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for n < 1, but safe to return 0 */

    seq_t val = 1;        /* a(1) */
    if (n == 1) return val;

    for (idx_t i = 2; i <= n; ++i) {
        val = rev_dec(val) + (seq_t)24;
    }
    return val;
}
