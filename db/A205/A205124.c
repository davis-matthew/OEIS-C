/*
 * A205124  Least s(k) such that n divides s(k)-s(j) for some j<k, where s(j)=
 *          j*(2^(j-1)).
 * https://oeis.org/A205124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 42, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A205124:  a(n) = the least s(k) such that n divides s(k)-s(j) for some j<k,
   where s(j) = j*2^(j-1).  We detect the first repeat of s(k) mod n. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition, but guard against zero. */
    if (n == 0) {
        return 0;
    }

    /* visited residues modulo n (0 … n-1).  Allocate as unsigned char. */
    unsigned char *seen = (unsigned char *)calloc((size_t)n, 1);
    if (!seen) {
        return 0;   /* allocation failed – unlikely in tests */
    }

    seq_t pow2 = 1;   /* 2^(k-1) for the current k, starting at k = 1 */

    for (idx_t k = 1;; ++k) {
        /* s(k) = k * 2^(k-1) */
        seq_t s = (seq_t)k * pow2;

        /* residue of s modulo n; result fits in idx_t because it is < n */
        idx_t r = (idx_t)(s % (seq_t)n);

        if (seen[r]) {
            /* first repeated residue → answer found */
            free(seen);
            return s;
        }
        seen[r] = 1;  /* mark this residue as seen */

        /* Prepare pow2 for next k: 2^k */
        pow2 = pow2 * 2;
    }
}
