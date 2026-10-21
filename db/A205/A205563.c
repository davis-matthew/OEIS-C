/*
 * A205563  Least positive integer k such that n divides 2k!-2j! for some j in
 *          [1,k-1].
 * https://oeis.org/A205563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least positive integer k such that n divides 2k! -
 *            2j! for some j with 1 ≤ j < k. The function searches for such a
 *            k by recording residues of factorial modulo n
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* helper: multiply modulo n using 128‑bit arithmetic */
static inline idx_t mod_mul_u64(idx_t a, idx_t b, idx_t mod) {
    return (idx_t)(((__uint128_t)a * b) % mod);
}

seq_t a(idx_t n)
{
    /* Handle the offset: a(1)=2 */
    if (n == 1) return (seq_t)2;

    /* n > 1 */
    bool even = (n & 1U) == 0;
    idx_t half = even ? n >> 1 : 0;

    /* Allocate a table of seen residues modulo n */
    uint8_t *seen = (uint8_t *)calloc((size_t)n, sizeof(uint8_t));
    if (!seen) return (seq_t)0;   /* memory failure – should never occur for n≤1e4 */

    /* current factorial modulo n */
    idx_t fact = 1 % n;   /* 1! % n */
    seen[fact] = 1;       /* record residue for j = 1 */

    idx_t k = 2;
    for (;;) {
        /* fact = k! % n */
        fact = mod_mul_u64(fact, k, n);

        /* Check if this residue matches any previous j! residue */
        if (seen[fact]) {
            free(seen);
            return (seq_t)k;
        }
        /* For even n, also need to check shifted residue */
        if (even && seen[(fact + half) % n]) {
            free(seen);
            return (seq_t)k;
        }

        /* Record this residue for future j's */
        seen[fact] = 1;
        ++k;
    }
}
