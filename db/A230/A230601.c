/*
 * A230601  a(n) = Lucas(2^n + 2).
 * https://oeis.org/A230601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   lucas_power2plus2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Lucas(2^n + 2)
 *
 * Computation:
 *   We compute the Fibonacci pair (F(k), F(k+1)) for k = 2^n + 2 using an
 *   iterative fast‑doubling method that scans the bits of k from most to
 *   least significant.  For this k the binary representation contains
 *   exactly two 1‑bits (except for n = 0, 1), so the loop is very small.
 *   The Lucas number is then obtained as L(k) = 2*F(k+1) – F(k).
 */

#include <stdbool.h>
#include "../utils/lucas_power2plus2.h"

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Public interface required by the grader.                               */
seq_t a(idx_t n)
{
    return lucas_power2plus2(n);
}
