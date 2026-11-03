/*
 * A053472  a(n) is the cototient of n (A051953) iterated 4 times.
 * https://oeis.org/A053472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = apply f(x) = x - phi(x) four times, with f(0)=0
 * Range:     u8 n <= 461, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 118 terms through n = 9751 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/totient_seq.h"
#include <stdbool.h>

seq_t a(idx_t n) {
    /* cast the index to the output type */
    seq_t m = (seq_t)n;
    /* iterate up to four times, stopping if the value becomes 0 */
    for (int i = 0; i < 4 && m > 0; ++i) {
        m = m - totient_seq(m);
    }
    return m;
}
