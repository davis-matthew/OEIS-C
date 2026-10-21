/*
 * A081608  Number of numbers <= n having no 0 in their ternary
 *          representation.
 * https://oeis.org/A081608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers 0 ≤ x ≤ n whose base‑3 representation
 *            contains no digit 0
 * Range:     u8 n <= 3280, u16 n <= 9617, u32 n <= 9617, u64 n <= 9617,
 *            u128 n <= 9617, big any n
 * Helpers:   pow2_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Convert n to base‑3 digits (least significant first). */
    idx_t tmp = n;
    int digits[64];
    int len = 0;
    bool hasZero = false;
    while (tmp > 0) {
        int d = tmp % 3;
        digits[len++] = d;
        if (d == 0)
            hasZero = true;
        tmp /= 3;
    }

    /* Count all numbers with fewer digits than len. */
    seq_t count = 0;
    if (len > 1)
        count = pow2_seq_t(len) - 2;        /* sum_{k=1}^{len−1} 2^k */

    /* Count numbers with the same length. */
    for (int pos = len - 1; pos >= 0; --pos) {
        int d = digits[pos];
        if (d == 0)                  /* cannot choose any valid digit here */
            break;
        if (d == 2)
            count += pow2_seq_t(pos);  /* choose 1 at this digit; rest arbitrary */
        /* if d == 1: must match, continue */
    }

    /* If n itself has no zero digit, include it. */
    if (!hasZero)
        count += 1;

    return count;
}
