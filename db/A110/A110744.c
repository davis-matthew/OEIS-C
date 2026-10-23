/*
 * A110744  Integers k such that k divides LS(k) or LS(k) divides k, where LS
 *          is the "Look and Say" function (A045918).
 * https://oeis.org/A110744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n‑th positive integer k such that k divides LS(k) or
 *            LS(k) divides k, where LS(k) is the look‑and‑say (run‑length
 *            encoding) of the decimal digits of k
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   look_and_say_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/look_and_say_uint64.h"

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but guard against underflow */

    uint64_t found = 0;
    uint64_t k = 1;
    uint64_t last = 0;

    while (found < n) {
        uint64_t ls = look_and_say_uint64(k);
        if (ls % k == 0 || k % ls == 0) {
            last = k;
            found++;
        }
        k++;
    }
    return (seq_t)last;
}
