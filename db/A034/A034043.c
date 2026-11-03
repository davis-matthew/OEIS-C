/*
 * A034043  Numbers that are imprimitively represented by x^2+y^2+z^2.
 * https://oeis.org/A034043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A034043: Numbers that are sum of three squares and are not squarefree
   (i.e., they have a square divisor >1). This function finds the nth term
   of that sequence in increasing order (a(1)=0). */

#include <stdbool.h>

/* test whether x has a non‑trivial square divisor (p^2 > 1) */
static bool has_nontrivial_square(seq_t x)
{
    if (x < 4) return false;           /* 1,2,3 are squarefree */
    for (idx_t i = 2; (seq_t)i * (seq_t)i <= x; ++i)
        if (x % ((seq_t)i * (seq_t)i) == 0)
            return true;
    return false;
}

/* test whether x can be represented as a sum of three squares
   (Legendre's three-square theorem) */
static bool sum_of_three_squares(seq_t x)
{
    if (x == 0) return true;           /* 0 = 0^2+0^2+0^2 */
    /* remove factors of 4 */
    while ((x & 3) == 0) x >>= 2;      /* x % 4 == 0 */
    return (x & 7) != 7;               /* x % 8 != 7 */
}

/* sequence entry a(n), 1‑based offset */
seq_t a(idx_t n)
{
    if (n == 1) return 0;              /* a(1) = 0 */

    idx_t count = 1;                   /* we already counted 0 */
    for (seq_t m = 1; ; ++m)
    {
        if (sum_of_three_squares(m) && has_nontrivial_square(m))
            ++count;
        if (count == n)
            return m;
    }
}
