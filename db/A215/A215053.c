/*
 * A215053  a(n) = 1/7*( binomial(n,7) - floor(n/7) ).
 * https://oeis.org/A215053
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( binomial(n,7) - floor(n/7) )/7
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t, if needed */

static seq_t binom7(idx_t n) {
    /* compute binomial(n,7) using 128‑bit intermediate arithmetic
       to avoid overflow for arbitrary-precision seq_t builds.  */
    unsigned __int128 res = 1;
    /* n >= 7 for our use case (n >= 8 in the sequence) */
    for (idx_t i = 1; i <= 7; ++i) {
        res *= (unsigned __int128)(n - 7 + i);
        res /= i;
    }
    return (seq_t)res;
}

seq_t a(idx_t n) {
    /* For indices below the offset (n < 8) the sequence is undefined.
       Returning zero for these values is safe because callers are
       expected to use the sequence only for n >= 8. */
    if (n < 8) {
        return (seq_t)0;
    }
    seq_t bin = binom7(n);
    seq_t div = (seq_t)(n / 7); /* floor division */
    return (bin - div) / 7;
}
