/*
 * A096662  Least nontrivial n-tuply triangular number.
 * https://oeis.org/A096662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3; for n>1, a(n)= a(n-1)*(a(n-1)+1)/2 (i.e. a(n)=T_{a(n-1)}
 *            where T_k=k(k+1)/2)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t, idx_t are provided by the build system */
seq_t a(idx_t n) {          /* external linkage, no static */
    /* At least n >= 1, but handle the trivial case gracefully */
    seq_t result = (seq_t)3;                /* a(1)=3 */
    if (n <= 1) return result;

    /* Iteratively compute a(n) = T_{a(n-1)} */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t low  = result;
        seq_t high = result + (seq_t)1;

        /* Divide the even factor by 2 first to keep the product small */
        if ((low & (seq_t)1) == 0) {
            low >>= 1;
        } else {
            high >>= 1;
        }

        result = low * high;        /* multiplication uses the type's operator */
    }
    return result;
}
