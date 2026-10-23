/*
 * A143051  Smallest number not occurring earlier and smaller than the largest
 *          square so far, the next square if no such number exists.
 * https://oeis.org/A143051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n = 0 then 0 else if n = 1 + k^2 then n + 2*k else n –
 *            1 (where k = floor(sqrt(n – 1))) This implementation uses an
 *            integer square‑root routine that works for all values that fit
 *            in idx_t (uint64_t). It does not rely on any external helper
 *            functions. The function is fully self‑contained...
 * Range:     u8 n <= 225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   square_root_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if the compiler does not provide it */
#include "../utils/square_root_u64.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }
    idx_t m = n - 1;                       /* n > 0, so safe */
    idx_t k = square_root_u64(m);
    if (k * k == m) {                     /* n = 1 + k^2 */
        idx_t res = n + 2 * k;
        return (seq_t)res;
    } else {
        idx_t res = n - 1;
        return (seq_t)res;
    }
}
