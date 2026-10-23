/*
 * A188038  a(n) = [n*r]-[k*r]-[n*r-k*r], where r=sqrt(2), k=2, [ ]=floor.
 * https://oeis.org/A188038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n·√2⌋ − ⌊2·√2⌋ − ⌊(n−2)·√2⌋ (with ⌊2·√2⌋ = 2). For n ≥ 3
 *            this simplifies to a(n) = ⌊n·√2⌋ + ⌊(n−2)·√2⌋ − 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 4264, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: ⌊√x⌋ for arbitrary‑size seq_t. */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low  = 0;
    seq_t high = x;
    seq_t ans  = 0;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;   /* mid = ⌊(low+high)/2⌋   */
        seq_t msq = mid * mid;
        if (msq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* ⌊k·√2⌋ for non‑negative k.  Uses ⌊√(2k²)⌋. */
static seq_t floor_sqrt2_pos(idx_t k)
{
    seq_t kk      = (seq_t)k * (seq_t)k;   /* k²                     */
    seq_t twokk   = kk * 2;                /* 2k²                    */
    return isqrt_seq(twokk);
}

seq_t a(idx_t n)
{
    if (n == 1)          /* ⌊1·√2⌋−⌊2·√2⌋−⌊−√2⌋ = 1  */
        return 1;
    if (n == 2)          /* ⌊2·√2⌋−⌊2·√2⌋−⌊0⌋ = 0   */
        return 0;

    seq_t fn   = floor_sqrt2_pos(n);      /* ⌊n·√2⌋                */
    seq_t fnm2 = floor_sqrt2_pos(n - 2);  /* ⌊(n−2)·√2⌋            */
    return fn - 2 - fnm2;                 /* ⌊n·√2⌋−2−⌊(n−2)·√2⌋     */
}
