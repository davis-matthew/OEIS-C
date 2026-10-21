/*
 * A095156  Least k such that 1/k begins with n after deleting the decimal
 *          point and the zeros following the decimal point.
 * https://oeis.org/A095156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least k such that the decimal expansion of 1/k begins
   with the decimal representation of n after deleting the decimal point
   and any leading zeros that follow it. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety, although offset is 1 */

    /* Extract decimal digits of n (most significant first). */
    seq_t tmp = (seq_t)n;
    int digs[64];
    int m = 0;
    while (tmp > 0) {
        digs[m++] = (int)(tmp % 10);
        tmp /= 10;
    }
    for (int i = 0; i < m / 2; ++i) {
        int t = digs[i];
        digs[i] = digs[m - 1 - i];
        digs[m - 1 - i] = t;
    }

    /* Search for the smallest k ≥ 2. */
    for (idx_t k = 2;; ++k) {
        seq_t rem = 1 % (seq_t)k;      /* remainder of 1/k (always 1 for k>1) */
        bool first_nonzero = false;
        int pos = 0;                   /* index of digit we are comparing */

        while (true) {
            unsigned long long prod = (unsigned long long)rem * 10ULL;
            seq_t digit = (seq_t)(prod / (unsigned long long)k);
            rem = (seq_t)(prod % (unsigned long long)k);

            if (!first_nonzero) {
                /* Skip any zeros immediately after the decimal point. */
                if (digit == 0) continue;
                first_nonzero = true;
                if (digit != (seq_t)digs[0]) break;
                if (m == 1) return (seq_t)k;
                pos = 1;
            } else {
                if (digit != (seq_t)digs[pos]) break;
                ++pos;
                if (pos == m) return (seq_t)k;
            }

            if (rem == 0) {
                /* The fraction has terminated; all remaining digits are zero. */
                if (first_nonzero) {
                    bool all_zero = true;
                    for (int j = pos; j < m; ++j)
                        if (digs[j] != 0) { all_zero = false; break; }
                    if (all_zero) return (seq_t)k;
                }
                break;  /* cannot match any longer */
            }
        }
    }
}
