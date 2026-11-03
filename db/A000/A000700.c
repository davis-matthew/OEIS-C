/*
 * A000700  Expansion of Product_{k>=0} (1 + x^(2k+1)); number of partitions
 *          of n into distinct odd parts; number of self-conjugate partitions;
 *          number of symmetric Ferrers graphs with n nodes.
 * https://oeis.org/A000700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   Product_{k>=1} (1 + x^(2*k-1)), coefficients of x^n
 * Range:     u8 n <= 63, u16 n <= 164, u32 n <= 487, u64 n <= 1605,
 *            u128 n <= 5684, big any n
 * Validated: standard, pass, 86 terms through n = 9981 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"


seq_t a(idx_t n) {
    seq_t *coeff = mem_new(seq_t, (n + 1));
    mem_set(coeff, 0, (n + 1) * sizeof(seq_t));
    coeff[0] = 1;
    
    for (idx_t d = 1; d <= n; d += 2) {
        for (idx_t i = n; i >= d; i--) {
            coeff[i] = coeff[i] + coeff[i - d];
        }
    }
    
    seq_t result = coeff[n];
    mem_free(coeff);
    return result;
}
