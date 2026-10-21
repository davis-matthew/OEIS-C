/*
 * A076159  Numbers k such that k = sod_4 - sod_3 + sod_2 - sod_1, where sod_j
 *          is the sum of j-th powers of digits of k.
 * https://oeis.org/A076159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full, base
 * Formula:   a(n) = k where k = Σ d(d‑1)(d²+1) over the decimal digits of k
 *            (equivalently k = sod₄ – sod₃ + sod₂ – sod₁)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const seq_t g_val[10] = {
    (seq_t)0,   (seq_t)0,   (seq_t)10,  (seq_t)60,
    (seq_t)204, (seq_t)520, (seq_t)1110, (seq_t)2100,
    (seq_t)3640, (seq_t)5904
};

static seq_t digit_contribution(seq_t x)
{
    seq_t sum = (seq_t)0;
    if (x == (seq_t)0) return sum;
    seq_t tmp = x;
    while (tmp > (seq_t)0) {
        uint8_t d = (uint8_t)(tmp % (seq_t)10);
        sum += g_val[d];
        tmp /= (seq_t)10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n <= (idx_t)0) return (seq_t)0;        /* undefined offset → 0 */
    if (n == (idx_t)1) return (seq_t)0;        /* first term is 0 */

    idx_t count = (idx_t)1;   /* count the zero term */
    idx_t i = (idx_t)1;

    while (count < n) {
        seq_t val = (seq_t)i;
        seq_t sum = digit_contribution(val);
        if (val == sum) {
            ++count;
            if (count == n) return val;
        }
        ++i;
    }
    /* unreachable */
    return (seq_t)0;
}
