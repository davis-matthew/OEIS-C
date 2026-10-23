/*
 * A023688  Numbers with exactly 6 ones in binary expansion.
 * https://oeis.org/A023688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 8008, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer whose binary representation contains
   exactly six 1‑bits.  The sequence is OEIS A023688.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static idx_t binom_exact(idx_t n, int k)
{
    /* Compute C(n,k) exactly using 128‑bit arithmetic to avoid
       overflow when multiplying intermediate products.  The value
       of n is small (n ≤ max position of 6‑bit numbers needed for
       n up to 10000), so the result comfortably fits in idx_t. */
    idx_t res = 1;
    for (int i = 1; i <= k; ++i) {
        unsigned __int128 tmp = (unsigned __int128)res * (n - k + i);
        tmp /= i;
        res = (idx_t)tmp;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1 – value for n=0 is unused */

    const int K = 6;                  /* exactly six 1‑bits */
    idx_t r = n - 1;                  /* zero‑based rank among K‑bit numbers */
    seq_t result = 0;

    for (int j = K; j >= 1; --j) {
        idx_t pos = j - 1;           /* minimal valid position for j bits */
        while (true) {
            idx_t next = pos + 1;
            idx_t val  = binom_exact(next, j);
            if (val <= r) {
                pos = next;
            } else {
                break;
            }
        }
        result += pow2(pos);         /* set the j‑th bit at position pos */
        r -= binom_exact(pos, j);    /* reduce remaining rank */
    }
    return result;
}
