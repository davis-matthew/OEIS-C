/*
 * A095043  An example of a (v,k,lambda)=(35,17,8) cyclic difference set.
 * https://oeis.org/A095043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095043: a(n) = the n-th element of a specific
 * cyclic difference set with parameters (v,k,λ) = (35,17,8).
 * The set is {0,1,3,4,7,9,11,12,13,14,16,17,21,27,28,29,33}.
 * Off‑by‑one offset: a(1)=0.
 */
#include <stddef.h>

static const seq_t diff_set[17] = {
    0, 1, 3, 4, 7, 9, 11, 12, 13, 14,
    16, 17, 21, 27, 28, 29, 33
};

seq_t a(idx_t n) {
    if (n < 1 || n > 17) return (seq_t)0;
    return diff_set[n-1];
}
