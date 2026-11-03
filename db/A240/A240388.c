/*
 * A240388  A sequence related to the Stern sequence s(n) (A002487), defined
 *          by w(n) = s(3n)/2.
 * https://oeis.org/A240388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = A002487(3*n) / 2 (Stern's diatomic sequence at 3n,
 *            halved)
 * Range:     u8 n <= 3525, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 145 terms through n = 8382 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the Stern diatomic sequence s(n).  s(0)=0, s(1)=1, s(2n)=s(n),
   s(2n+1)=s(n)+s(n+1).  This iterative implementation runs in O(log n). */
static seq_t stern_of(idx_t n) {
    if (n == 0)
        return 0;

    seq_t a = 1, b = 1;          /* start with s(1)=1, s(2)=1 */

    /* Find floor(log2 n) by counting bits above the least significant bit. */
    int p = 0;
    idx_t tmp = n >> 1;
    while (tmp) {
        ++p;
        tmp >>= 1;
    }

    /* Process bits of n from second‑most significant down to the least. */
    for (int i = p - 1; i >= 0; --i) {
        if ((n >> i) & 1)
            a = a + b;   /* odd position: s(2m+1) = s(m)+s(m+1) */
        else
            b = a + b;   /* even position: s(2m)   = s(m)     */
    }
    return a;               /* s(n) = a after all bit steps */
}

/* Main entry for OEIS A240388: w(n) = s(3n)/2. */
seq_t a(idx_t n) {
    idx_t k = n * 3;          /* 3*n, safe for typical n values in tests  */
    seq_t val = stern_of(k);  /* compute s(3n) */
    return val / 2;           /* exact division by 2 (s(3n) is always even) */
}
