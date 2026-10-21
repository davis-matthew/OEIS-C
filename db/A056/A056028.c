/*
 * A056028  Numbers k such that k^18 == 1 (mod 19^2).
 * https://oeis.org/A056028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 3267, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A056028:  Numbers k such that k^18 ≡ 1 (mod 19^2).
 * The recurrence is a(n) = a(n‑18) + 19^2 for n > 18.
 * The first 18 values are provided explicitly.
 * Formula implementation: a(1..18) given, a(n) = a(n‑18)+361 for n>18.
 */

#include <stdint.h>

static const seq_t first18[18] = {
    1, 28, 54, 62, 68, 69, 99, 116, 127, 234,
    245, 262, 292, 293, 299, 307, 333, 360
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n==0, return 0 */
    idx_t idx = n - 1;                /* 0‑based index (n=1 → idx=0) */
    idx_t period  = idx % 18;         /* position within the 18‑term pattern */
    idx_t cycles  = idx / 18;         /* number of complete 18‑term cycles passed */

    /* add 361 for each complete cycle */
    seq_t add = (seq_t)361 * (seq_t)cycles;

    /* first18[period] is a(1) … a(18) */
    seq_t result = first18[(unsigned int)period] + add;
    return result;
}
