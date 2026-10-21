/*
 * A228829  a(n) = (m+n-k) mod (m-n+k) where k = BigOmega(n) and m is the next
 *          larger integer after n with the same k = BigOmega(m) as n.
 * https://oeis.org/A228829
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (m+n-k) mod (m-n+k), where k = BigOmega(n) and m > n is
 *            the smallest integer with BigOmega(m) = k
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n) {
    /* The sequence is defined for n ≥ 2. For n < 2 the value is undefined,
       but this implementation simply returns 0. */
    if (n < 2)
        return 0;

    /* k = BigOmega(n) */
    seq_t k = bigomega(n);

    /* Find the next integer m > n with the same BigOmega. */
    idx_t m = n + 1;
    while (bigomega(m) != k)
        ++m;

    /* Compute (m + n - k) mod (m - n + k). */
    seq_t mn   = (seq_t)m;
    seq_t nseq = (seq_t)n;
    seq_t num  = mn + nseq - k;
    seq_t den  = mn - nseq + k;
    return num % den;
}
