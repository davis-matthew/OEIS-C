/*
 * A081611  Number of numbers <= n having no 2 in their ternary
 *          representation.
 * https://oeis.org/A081611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers ≤ n whose base‑3 representation
 *            contains no digit 2
 * Range:     u8 n <= 3279, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a digit‑DP over base‑3; runs in O(log₃ n). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert n to base‑3 digits (least significant first). */
    idx_t digits[64];
    int len = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = tmp % 3;
            tmp /= 3;
        }
    }

    seq_t ans = 0;
    bool broken = false;

    /* Process digits from most significant to least. */
    for (int i = len - 1; i >= 0; --i) {
        idx_t digit   = digits[i];
        int remaining = i;   /* digits after this position */

        if (digit == 2) {
            /* Choosing 0 or 1 at this position gives all numbers < n.
               Each choice allows 2^(remaining) completions. */
            ans += (seq_t)2 * (((seq_t)1) << remaining);
            broken = true;
            break;            /* further digits cannot match n */
        } else if (digit == 1) {
            /* Choosing 0 yields all numbers < n with the rest arbitrary. */
            ans += ((seq_t)1) << remaining;
            /* continue tight with digit 1 */
        } /* digit == 0 → no smaller choice, keep tight */
    }

    if (!broken) {
        /* n itself is valid (all digits are 0 or 1). */
        ans += 1;
    }

    return ans;
}
