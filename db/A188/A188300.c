/*
 * A188300  a(n) = [n*r] - [k*r] - [n*r-k*r], where r=1/sqrt(2), k=3, [
 *          ]=floor.
 * https://oeis.org/A188300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ n/√2 ⌋ – ⌊ (n–3)/√2 ⌋ – ⌊ 3/√2 ⌋ (k = 3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 149 terms through n = 9596 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* r = 1/√2,  a(1) is the first listed term.                         */

#include <stdint.h>

/* Compute ⌊ n/√2 ⌋ for n ≥ 0 using only integer arithmetic.
   The routine uses a binary search on t with the invariant
   2·t² ≤ n², which is equivalent to t ≤ n/√2.
   128‑bit arithmetic is used only in the intermediate products,
   not in the return type.                                         */
static inline seq_t floor_div_by_sqrt2_uint(idx_t n)
{
    if (n == 0) return 0;

    idx_t lo = 0;
    idx_t hi = n;
    while (lo < hi) {
        /* lower‑bound mid to avoid infinite loop.                 */
        idx_t mid = lo + ((hi - lo + 1) >> 1);

        unsigned __int128 mid_sq = (unsigned __int128)mid * mid;   /* mid² */
        unsigned __int128 n_sq  = (unsigned __int128)n  * n;      /* n²  */

        /* 2·mid² ≤ n²   ⇔   mid² ≤ ⌊n²/2⌋                         */
        unsigned __int128 half = n_sq >> 1;                       /* ⌊n²/2⌋ */

        if (mid_sq <= half)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (seq_t)lo;
}

/* Sequence A188300 */
seq_t a(idx_t n)
{
    const seq_t K = 2;            /* ⌊ 3/√2 ⌋  → 2 */

    seq_t fn = floor_div_by_sqrt2_uint(n);

    if (n >= 3) {
        seq_t fmk = floor_div_by_sqrt2_uint(n - 3);
        /* fn - fmk - K is 0 or 1 */
        return fn - fmk - K;
    } else {
        /* n = 1 or 2: (n–3) is negative, use the equivalent formula
           fn + ⌊ |n–3|/√2 ⌋ – 1  which results again in 0 or 1.      */
        idx_t m   = 3 - n;                        /* 2 or 1 */
        seq_t fm  = floor_div_by_sqrt2_uint(m);
        return fn + fm - 1;
    }
}
