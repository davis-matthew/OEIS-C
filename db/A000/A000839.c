/*
 * A000839  Number of n-input 3-output switching networks under action of
 *          complementing group on the inputs and outputs.
 * https://oeis.org/A000839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(3*(2^n-1) - n) + 2^(2^n + 2^(n-1)) - 2^(2^n + 2^(n-1)
 *            - n)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a000809_p2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000809_p2.h"


seq_t a(idx_t n) {
    seq_t p2_n_1 = a000809_p2(n - 1);
    seq_t p2_n = a000809_p2(n);
    
    seq_t base_exp = p2_n + p2_n_1;
    seq_t exp1 = 3 * p2_n - 3 - (seq_t)n;
    seq_t exp3 = base_exp - (seq_t)n;
    
    seq_t t1 = a000809_p2((idx_t)exp1);
    seq_t t2 = a000809_p2((idx_t)base_exp);
    seq_t t3 = a000809_p2((idx_t)exp3);
    
    return t1 + t2 - t3;
}
