/*
 * A143315  Triangle read by rows: T(n, k) = 2*A126988(n, k) -
 *          signum(A126988(n, k)).
 * https://oeis.org/A143315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   _row_of_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A143315:  Triangle read by rows  
 *   T(n,k) = 2*(n/k) – 1  if n%k==0, otherwise 0.
 *   a(n) is the flattening of T, with rows starting at n=1. */

/* The function assumes idx_t == uint64_t and seq_t is any unsigned
 * integer type (native or arbitrary‑precision).  It works without
 * external helpers. */

#include <stddef.h>
#include "../utils/_row_of_n.h"

seq_t a(idx_t n) {
    /* Find row r and column k for the flattened index n */
    idx_t r = _row_of_n(n);
    seq_t prev_tri_seq = (seq_t)(r - 1) * (seq_t)r / 2;
    idx_t k = n - (idx_t)prev_tri_seq;   /* 1 <= k <= r */

    /* If n is not divisible by k, the entry is 0 */
    if (r % k != 0) {
        return 0;
    }

    /* Otherwise compute 2*(n/k)-1 */
    idx_t v = r / k;               /* n/k */
    return (seq_t)v * 2 - 1;
}
