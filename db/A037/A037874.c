/*
 * A037874  (1/2)*Sum{|d(i)-e(i)|}, where Sum{d(i)*6^i} is base 6
 *          representation of n and e(i) are digits d(i) in nonincreasing
 *          order, for i=0,1,...,m.
 * https://oeis.org/A037874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * Σ |d_i(n) – e_i(n)| where d_i(n) are the base‑6
 *            digits of n (least‑significant first) and e_i(n) are those
 *            digits sorted in non‑increasing order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the case n == 0 – not used in the OEIS data,
       but keeps the routine total.  */
    if (n == 0) return 0;

    /* Base‑6 representation of n.  At most 20 digits for 64‑bit n. */
    uint8_t d[64];
    uint8_t e[64];
    int len = 0;

    idx_t tmp = n;
    while (tmp > 0) {
        d[len] = (uint8_t)(tmp % 6);
        e[len] = d[len];
        tmp  /= 6;
        ++len;
    }

    /* Sort e[] in non‑increasing order (descending).  Simple bubble sort
       is fine as len is very small (≤ 20 for 64‑bit input).  */
    for (int i = 0; i < len - 1; ++i) {
        for (int j = 0; j < len - 1 - i; ++j) {
            if (e[j] < e[j + 1]) {
                uint8_t tmp = e[j];
                e[j] = e[j + 1];
                e[j + 1] = tmp;
            }
        }
    }

    /* Compute Σ |d_i – e_i|.  Since each |d_i – e_i| ≤ 5 and len ≤ 20,
       this sum fits comfortably in an unsigned 64‑bit integer.  */
    seq_t sum = 0;
    for (int i = 0; i < len; ++i) {
        seq_t diff = (d[i] > e[i]) ? (d[i] - e[i]) : (e[i] - d[i]);
        sum += diff;
    }

    /* The definition requires a division by 2.  The sum is always even,
       because each pair of digits contributes twice to the sum.  */
    return sum / (seq_t)2;
}
