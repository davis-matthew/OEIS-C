/*
 * A037880  (1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*3^i} is base 3
 *          representation of n and e(i) are digits d(i) in nondecreasing
 *          order.
 * https://oeis.org/A037880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * Σ_i |d_i - e_i| where d_i are the base‑3 digits
 *            of n (least significant first) and e_i are those digits sorted
 *            in nondecreasing order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n = 0 (outside OEIS offset) */
    if (n == 0) return 0;

    /* Base‑3 digits of n, least significant first.
       n < 2^64 ⇒ at most 41 digits in base 3. */
    int digits[64];
    int len = 0;
    uint64_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 3);
        tmp /= 3;
    }

    /* Count occurrences of each digit 0,1,2 */
    int c0 = 0, c1 = 0, c2 = 0;
    for (int i = 0; i < len; ++i) {
        switch (digits[i]) {
            case 0: ++c0; break;
            case 1: ++c1; break;
            case 2: ++c2; break;
        }
    }

    /* Build sorted digits array e (nondecreasing) */
    int sorted[64];
    int idx = 0;
    for (int i = 0; i < c0; ++i) sorted[idx++] = 0;
    for (int i = 0; i < c1; ++i) sorted[idx++] = 1;
    for (int i = 0; i < c2; ++i) sorted[idx++] = 2;

    /* Sum absolute differences */
    seq_t sum = 0;
    for (int i = 0; i < len; ++i) {
        int d = digits[i];
        int e = sorted[i];
        int diff = d - e;
        if (diff < 0) diff = -diff;
        sum += (seq_t)diff;
    }

    return sum >> 1;   /* divide by 2 */
}
