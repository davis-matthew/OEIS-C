/*
 * A122633  F(n)_n where F() = Fibonacci numbers A000045.
 * https://oeis.org/A122633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret the decimal string of the n‑th Fibonacci number
 *            F(n) (OEIS A000045) as a number written in base n. In this way
 *            a(n) = Σ_{k=0}^{m-1} dₖ · n^k where dₖ are the decimal digits of
 *            F(n)
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   fib, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/fib.h"
#include "../utils/seq_to_str.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; this case is irrelevant */
    seq_t fn = fib(n);                /* F(n) */
    /* buffer big enough for F(10000) which has ~2089 decimal digits */
    char buf[4000];
    size_t len;
    seq_to_str(fn, buf, &len);

    seq_t result = 0;
    seq_t base   = (seq_t)n;
    for (size_t i = 0; i < len; ++i) {
        result = result * base + (seq_t)(buf[i] - '0');
    }
    return result;
}
