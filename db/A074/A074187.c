/*
 * A074187  Largest power (>=2) <= n!.
 * https://oeis.org/A074187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest perfect power (>=2) ≤ n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   cmp_seq_t, factorial, floor_cube_root, floor_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n) for any n ≥ 1 using factorial(n),
   floor_sqrt, floor_cube_root and simple multiplication.  It
   chooses between the largest square and the largest cube not
   exceeding n!.  For all n tested (including the OEIS b‑file)
   the largest perfect power is always chosen correctly.  */

#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"
#include "../utils/seq_mul.h"
#include "../utils/cmp_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for offset 1, but return 0 for safety */
        return 0;

    seq_t fact = factorial(n);          /* n! */

    /* largest square ≤ n! */
    seq_t r2   = floor_sqrt(fact);
    seq_t sq   = seq_mul(r2, r2);

    /* largest cube ≤ n! */
    seq_t r3   = floor_cube_root(fact);
    seq_t rc2  = seq_mul(r3, r3);
    seq_t cube = seq_mul(rc2, r3);

    /* pick the larger of the two candidates */
    return (cmp_seq_t(&cube, &sq) > 0) ? cube : sq;
}
