/*
 * A232486  a(1) = 3; thereafter a(n+1) = a(n) + product of nonzero digits of
 *          a(n).
 * https://oeis.org/A232486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 3; thereafter a(n+1) = a(n) + product of non‑zero digits
 *            of a(n)
 * Range:     u8 n <= 22, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* for size_t */
#include <stdint.h>    /* for uint64_t */

static inline seq_t prod_nonzero_digits(seq_t x)
{
    seq_t prod = 1;
    while (x > 0) {
        int d = (int)(x % 10);   /* digit is <10, safe cast */
        if (d != 0)
            prod *= (seq_t)d;
        x /= 10;
    }
    return prod;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    seq_t val = 3;               /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t inc = prod_nonzero_digits(val);
        val += inc;
    }
    return val;
}
