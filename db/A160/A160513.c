/*
 * A160513  Positive squarefree integers n such that, if n is the m-th
 *          squarefree integer (i.e., the m-th term of A005117), then n is
 *          also the m-th term of A000961 (the prime powers).
 * https://oeis.org/A160513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A160513: finite list 1, 2, 3, 7, 17, 19. Returns 0 for n > 6. */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t tbl[] = { 1, 2, 3, 7, 17, 19 };
    size_t len = sizeof(tbl) / sizeof(tbl[0]);

    if (n >= 1 && (idx_t)len >= n)
        return tbl[n - 1];
    return 0;
}
