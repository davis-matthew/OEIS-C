/*
 * A127610  a(n) = floor(( (n+1)/2 )^n) - n!.
 * https://oeis.org/A127610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(((n+1)/2)^n) - n!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* For very small seq_t types (≤64 bits) we use a 128‑bit intermediate
       to avoid overflowing while computing (n+1)^n.  When seq_t is
       larger (or an arbitrary‑precision class) we rely on the provided
       big_pow routine. */
    seq_t power_term;
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 num = 1;
        uint64_t base = (uint64_t)(n + 1);
        for (idx_t i = 0; i < n; ++i)
            num *= base;
        /* Division by 2^n is a right shift; truncation gives floor. */
        num >>= n;
        power_term = (seq_t)num;
    } else {
        power_term = big_pow((seq_t)(n + 1), (seq_t)n) / big_pow((seq_t)2, (seq_t)n);
    }
    seq_t fact = factorial(n);
    return power_term - fact;
}
