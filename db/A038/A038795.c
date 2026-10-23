/*
 * A038795  T(n,n-5), array T as in A038792.
 * https://oeis.org/A038795
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..5} binomial(n-k, k) for n ≥ 9; a(5)=1, a(6)=6,
 *            a(7)=17, a(8)=33. (OEIS A038795)
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* declares seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* Undefined for n < 5 – return 0 in that case. */
    if (n < 5) return (seq_t)0;

    /* The closed formula is valid only for n ≥ 9.  The first four
       values are supplied explicitly. */
    if (n < 9) {
        static const seq_t base[] = {1, 6, 17, 33};  /* a(5)..a(8) */
        return base[n - 5];
    }

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= 5; ++k) {
        /* n >= 9 guarantees (n-k) ≥ 4, so binomial is defined. */
        sum += binomial(n - k, k);
    }
    return sum;
}
