/*
 * A162898  a(n) = A162897(n)^A162897(n+1).
 * https://oeis.org/A162898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   aux, big_pow
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
#include "../utils/big_pow.h"
#include "../utils/aux.h"

/* Main sequence function */
seq_t
a(idx_t n)
{
    seq_t base = aux(n);
    seq_t exp  = aux(n + 1);
    return big_pow(base, exp);
}
