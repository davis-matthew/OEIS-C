/*
 * A018399  Divisors of 336.
 * https://oeis.org/A018399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 336 (0 if n > 20)
 * Range:     u8 n <= 19, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 336 = 2^4 * 3^1 * 7^1 → 20 divisors. */
    static const seq_t divs[20] = {
        (seq_t)1,  (seq_t)2,  (seq_t)3,  (seq_t)4,
        (seq_t)6,  (seq_t)7,  (seq_t)8,  (seq_t)12,
        (seq_t)14, (seq_t)16, (seq_t)21, (seq_t)24,
        (seq_t)28, (seq_t)42, (seq_t)48, (seq_t)56,
        (seq_t)84, (seq_t)112, (seq_t)168, (seq_t)336
    };

    /* idx_t may be larger than 20; return 0 for out‑of‑range indices. */
    if (n < 1 || n > 20)
        return (seq_t)0;
    return divs[(size_t)(n - 1)];
}
