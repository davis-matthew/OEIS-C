/*
 * A216789  Table read by antidiagonals: T(n,k) is the digital sum of k in
 *          base n displayed in decimal.
 * https://oeis.org/A216789
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9816, u16 n <= 9816, u32 n <= 9816, u64 n <= 9816,
 *            u128 n <= 9816, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Algorithm: a(n) is the antidiagonal reading of the table  
   T(m,k) = sum of the base‑m digits of k.
   For n ≥ 2:
       let t  = n – 2 (zero‑based position in the sequence)
       find the smallest integer a ≥ 1 such that a·(a+1)/2 > t
       then the diagonal sum s = a + 1
       offset   = t – (a–1)·a/2          (0 ≤ offset < s‑1)
       base_m   = s – offset
       k        = offset
       answer   = sum of the base‑base_m digits of k
*/

seq_t a(idx_t n)
{
    if (n < (idx_t)2)
        return 0;                      /* value for indices < 2 is irrelevant */

    uint64_t t = n - 2;                /* zero‑based index in the sequence */

    /* Find minimal a ≥ 1 with a·(a+1)/2 > t using binary search */
    uint64_t low  = 1;
    uint64_t high = UINT64_MAX;
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val > (unsigned __int128)t)
            high = mid;
        else
            low  = mid + 1;
    }
    uint64_t a = low;                              /* guaranteed a·(a+1)/2 > t */
    uint64_t s = a + 1;                            /* diagonal sum n + k */

    unsigned __int128 prev128 = (unsigned __int128)(a - 1) * a / 2;
    uint64_t prev   = (uint64_t)prev128;           /* elements before this diagonal */
    uint64_t offset = t - prev;                   /* offset inside the diagonal   */
    uint64_t base_n = s - offset;                 /* base n for the current term   */

    seq_t k    = (seq_t)offset;                   /* value whose digits we sum    */
    seq_t base = (seq_t)base_n;

    seq_t digit_sum = 0;
    while (k > 0) {
        digit_sum += k % base;
        k /= base;
    }
    return digit_sum;
}
