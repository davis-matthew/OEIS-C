/*
 * A166756  Number of nX2 1..3 arrays containing at least one of each value,
 *          and all equal values connected.
 * https://oeis.org/A166756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n^4 - 8*n^3 + 11*n^2 - 13*n + 6 (for n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n) {
    /* cast to seq_t first to avoid overflow in intermediate idx_t arithmetic */
    seq_t nn = (seq_t)n;
#if defined(__SIZEOF_INT128__) || defined(_MSC_VER)
    /* use 128‑bit arithmetic when available to keep the intermediate result exact */
    unsigned __int128 t = (unsigned __int128)nn;
    unsigned __int128 res = 4*t*t*t*t
                          - 8*t*t*t
                          + 11*t*t
                          - 13*t
                          + 6;
    return (seq_t)res;
#else
    /* fallback: compute using seq_t arithmetic, assuming the result fits in seq_t */
    return 4*nn*nn*nn*nn
         - 8*nn*nn*nn
         + 11*nn*nn
         - 13*nn
         + 6;
#endif
}
