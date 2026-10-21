/*
 * A066333  a(n) = min(x : x^2 + n^2 = 0 mod (x+n-1)).
 * https://oeis.org/A066333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{x : x^2 + n^2 ≡ 0 (mod (x+n-1))}
 * Range:     u8 n <= 11, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal solution derived from divisor of D = 2*n^2 - 2*n + 1 */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    /* D = 2*n^2 - 2*n + 1 */
    seq_t D = 2 * n_seq * n_seq - 2 * n_seq + 1;

    /* Find the smallest divisor d of D that is at least n. */
    seq_t best = D;                /* fallback: D itself */
    for (seq_t d = 1; d * d <= D; ++d) {
        if (D % d == 0) {
            seq_t d1 = d;
            seq_t d2 = D / d;
            if (d1 >= n_seq && d1 < best) best = d1;
            if (d2 >= n_seq && d2 < best) best = d2;
        }
    }

    /* Result x = d - n + 1 */
    return best - n_seq + 1;
}
