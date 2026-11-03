/*
 * A062289  Numbers n such that n-th row in Pascal triangle contains an even
 *          number, i.e., A048967(n) > 0.
 * https://oeis.org/A062289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer ≥2 that is NOT of the form 2^k‑1 (i.e.
 *            its binary representation contains the substring “10”)
 * Range:     u8 n <= 247, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 87 terms through n = 8297 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool has_all_ones(seq_t v)
{
    /* v is of the form 2^k‑1  <=>  (v & (v+1)) == 0  */
    seq_t w = v + (seq_t)1;
    return (v & w) == (seq_t)0;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0; /* undefined for n<1 */

    seq_t count = (seq_t)0;
    seq_t num   = (seq_t)1;           /* start from 2 */

    /* search until the n‑th qualifying number is found */
    while (1) {
        ++num;
        if (has_all_ones(num)) /* skip numbers 2^k‑1 (including 1) */
            continue;
        ++count;
        if (count == n)
            return num;
    }
}
