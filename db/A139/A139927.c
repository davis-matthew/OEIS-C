/*
 * A139927  Primes of the form 19x^2+14xy+19y^2.
 * https://oeis.org/A139927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime of the form 19x²+14xy+19y². Equivalently,
 *            the primes are exactly those congruent to 19, 67, 115, 163, 187,
 *            or 307 modulo 312
 * Range:     u8 n <= 3, u16 n <= 400, u32 n <= 271, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_target_residue
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_target_residue.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined for offset 1 */

    /* persistent static data: list of primes of the target form */
    static seq_t *cands      = NULL;
    static size_t  cands_cap = 0;
    static size_t  cands_used = 0;
    static seq_t  next_start = 2;      /* first integer to test */

    /* initialise storage on first call */
    if (!cands) {
        cands_cap = 16;
        cands     = (seq_t*)malloc(cands_cap * sizeof(seq_t));
        if (!cands) return 0; /* allocation failure */
    }

    /* generate primes until we have at least n terms */
    while (cands_used < n) {
        for (;;) {
            seq_t candidate = next_start++;
            if (!is_target_residue(candidate))
                continue;
            if (!is_prime_big(candidate))
                continue;

            /* grow array if needed */
            if (cands_used == cands_cap) {
                cands_cap <<= 1;               /* double capacity */
                seq_t *tmp = (seq_t*)realloc(cands, cands_cap * sizeof(seq_t));
                if (!tmp) return 0;            /* allocation failure */
                cands = tmp;
            }
            cands[cands_used++] = candidate;
            break;
        }
    }

    return cands[n - 1];
}
