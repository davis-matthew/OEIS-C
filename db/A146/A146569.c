/*
 * A146569  Numbers m with the property that shifting the rightmost digit of m
 *          to the left end multiplies the number by 4.
 * https://oeis.org/A146569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 6, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: For n > 0 let
 *   k = n / 6
 *   r = n % 6   (if r == 0 set r = 6 and k = k - 1)
 * Then
 *   a(n) = a(r) * Σ_{i=0}^{k} 10^(6*i)
 * with a(1)=102564, a(2)=128205, a(3)=153846, a(4)=179487,
 * a(5)=205128, a(6)=230769 and a(0)=0.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle offset */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Base values for the first 6 terms */
    static const seq_t base[6] = {
        (seq_t)102564,
        (seq_t)128205,
        (seq_t)153846,
        (seq_t)179487,
        (seq_t)205128,
        (seq_t)230769
    };

    idx_t k = n / 6;      /* number of 6‑digit blocks after the first one */
    idx_t r = n % 6;      /* index within the block, 0..5 */
    if (r == 0) {
        r = 6;
        --k;
    }

    /* Compute multiplier = Σ_{i=0}^{k} 10^(6*i) */
    seq_t pow = (seq_t)1;    /* 10^(6*i) */
    seq_t mult = (seq_t)1;   /* start with 1 for i=0 */
    for (idx_t i = 0; i < k; ++i) {
        pow *= (seq_t)1000000;   /* multiply by 10^6 */
        mult += pow;
    }

    /* Result */
    return base[r - 1] * mult;
}
