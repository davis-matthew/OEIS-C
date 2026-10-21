/*
 * A215531  The limit of the string "0, 1" under the operation 'append first k
 *          terms, k=k+2' with k=1 initially.
 * https://oeis.org/A215531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A215531  :  Limit of the word formed by repeatedly appending the
 *  first k terms of the current word, with k starting at 1 and
 *  increasing by 2 each time.
 *
 *  Formula (recursive):
 *      a(0) = 0,   a(1) = 1
 *      for n ≥ 2:
 *          r = ⌊√(n‑2)⌋
 *          L = r² + 2          // length of the word before the last append
 *          a(n) = a(n – L)
 *
 *  The iteration stops when n becomes 0 or 1.
 */

/*  Integer square‑root of an unsigned 64‑bit integer.
 *  Returns ⌊√n⌋.  Implements a simple binary search; safe for any n
 *  that fits in uint64_t.  Uses unsigned __int128 for the product
 *  to avoid overflow. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = 0xFFFFFFFFULL;          /* √(2⁶⁴‑1) < 2³² */
    idx_t mid, res = 0;

    while (low <= high) {
        mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t pos = n;
    while (pos >= 2) {
        idx_t r = floor_sqrt_u64(pos - 2U);  /* r = ⌊√(pos‑2)⌋   */
        idx_t L = r * r + 2U;                /* length before last append */
        pos -= L;                           /* step back to the earlier word   */
    }
    return (seq_t)pos;                       /* pos is 0 or 1                   */
}
