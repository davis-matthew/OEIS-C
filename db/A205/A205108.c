/*
 * A205108  Least s(k) such that n divides s(k)-s(j) for some j<k, where s(j)
 *          = 3^j-2^j = A001047(j).
 * https://oeis.org/A205108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n)=least s(k) such that n divides s(k)-s(j) for some j<k,
   where s(j)=3^j-2^j.  The returned value is s(k). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;  /* undefined for n=0, return 0 */

    /* Allocate array to remember which remainders have appeared. */
    size_t cap = (size_t)n;
    unsigned char *seen = (unsigned char *)calloc(cap, sizeof(unsigned char));
    if (!seen) return (seq_t)0;  /* allocation failure */

    /* Recurrent computation of 3^k and 2^k */
    seq_t pow3 = (seq_t)3;
    seq_t pow2 = (seq_t)2;

    /* Remainders modulo n, stored as idx_t for speed */
    idx_t r3 = (idx_t)(3 % n);
    idx_t r2 = (idx_t)(2 % n);

    for (idx_t k = 1;; ++k) {
        /* Compute remainder of s(k)=pow3-pow2 modulo n */
        idx_t r;
        if (r3 >= r2) r = r3 - r2;
        else r = r3 + n - r2;

        if (seen[(size_t)r]) {
            /* Found repeat; s(k)=pow3-pow2 is the answer */
            seq_t result = pow3 - pow2;
            free(seen);
            return result;
        }
        seen[(size_t)r] = 1;

        /* Prepare next terms: pow3 *=3; pow2 *=2 */
        pow3 = pow3 * (seq_t)3;
        pow2 = pow2 * (seq_t)2;

        /* Update small‑moduli remainders */
        r3 = (idx_t)((r3 * (idx_t)3) % n);
        r2 = (idx_t)((r2 * (idx_t)2) % n);
    }
}
