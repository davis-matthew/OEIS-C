/*
 * A000842  Number of n-input 3-output switching networks under action of
 *          symmetric group S(n) on the inputs and complementing group C(3,2)
 *          on the outputs.
 * https://oeis.org/A000842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(8*n!)) * Σ_{σ ∈ S_n} 8^{c(σ)}, where c(σ) is the
 *            number of cycles of σ acting on {0,1}^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   count_cycles_for_perm, factorial_seq, gen_perms_recursive,
 *            pow_eight
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/count_cycles_for_perm.h"
#include "../utils/factorial_seq.h"
#include "../utils/gen_perms_recursive.h"
#include "../utils/pow_eight.h"

seq_t a(idx_t n) {
    if (n == 0 || n > 20) return 0;
    
    idx_t *perm = (idx_t *)malloc(n * sizeof(idx_t));
    for (idx_t i = 0; i < n; i++) {
        perm[i] = i;
    }
    
    seq_t sum_value = 0;
    gen_perms_recursive(perm, n, 0, &sum_value);
    
    free(perm);
    
    seq_t n_fact = factorial_seq(n);
    seq_t denom = 8 * n_fact;
    
    return sum_value / denom;
}
