/*
 * A018753  Divisors of 975.
 * https://oeis.org/A018753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 975 in increasing order.
   a(n) is the n‑th divisor of 975 (a(1)=1, a(2)=3, ..., a(12)=975).
   For n > 12 the sequence is undefined, so a(n) returns 0. */
#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* 975 is a small constant; we simply enumerate all its divisors
       and return the n‑th one.  This works for any seq_t type that
       supports the usual arithmetic operators. */
    idx_t cnt = 0;
    const idx_t limit = 975u;
    for (idx_t i = 1; i <= limit; ++i)
    {
        if (((seq_t)limit) % ((seq_t)i) == 0)
        {
            ++cnt;
            if (cnt == n)
                return (seq_t)i;
        }
    }
    /* If n exceeds the number of divisors, return 0. */
    return (seq_t)0;
}
