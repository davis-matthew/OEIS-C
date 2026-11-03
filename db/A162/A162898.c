/*
 * A162898  a(n) = A162897(n)^A162897(n+1).
 * https://oeis.org/A162898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A162898: a(n) = A162897(n)^A162897(n+1)
   where A162897(n) is given piecewise by
        n = 1 or 2 → 2
        n = 3       → 3
        n = 5       → 4
        n even (n ≥ 4)   → n/2
        n odd  (n ≥ 7)   → (n+1)/2   */

/* Computes the auxiliary sequence A162897(n) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

static seq_t
aux(idx_t n)
{
    if (n <= 2)              /* n = 1 or 2 */
        return (seq_t)2;
    if (n == 3)
        return (seq_t)3;
    if (n == 5)
        return (seq_t)4;
    if ((n & 1) == 0)        /* even n ≥ 4 */
        return (seq_t)(n / 2);
    else                     /* odd n ≥ 7 */
        return (seq_t)((n + 1) / 2);
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    seq_t base = aux(n);
    seq_t exp  = aux(n + 1);
    return ipow(base, exp);
}
