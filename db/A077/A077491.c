/*
 * A077491  a(n) = smallest k such that 2k has digit sum = n.
 * https://oeis.org/A077491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = smallest k such that the digit sum of 2k equals n
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: find the smallest even integer M with digit sum n,
   then return M/2.  M is constructed greedily from most significant
   digits, ensuring that the last digit is even.  The function is
   efficient for n up to at least 10000. */

#include <stdbool.h>

/* Helper: test whether a sum can be distributed over 'rem' positions,
   where the last position (if any) must contain an even digit <= 8. */
static bool feasible(int rem, int sum) {
    if (sum < 0) return false;
    if (rem == 0) {
        return (sum <= 9) && ((sum & 1) == 0);
    }
    if (rem == 1) {
        return (sum <= 9) && ((sum & 1) == 0);
    }
    /* For rem >= 2: try all even last digits 0,2,4,6,8. */
    for (int e = 0; e <= 8; e += 2) {
        int rest = sum - e;
        if (rest < 0) continue;
        if (rest <= 9 * (rem - 1)) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset 1: not used, but safe */

    /* Work with a 32‑bit integer for summation; n is reasonably small. */
    int sum = (int)n;
    /* Start with the minimal possible number of digits. */
    int len = (sum + 8) / 9;        /* ceil(sum/9) */

    seq_t M = 0;                    /* minimal even number with sum 'sum' */

    while (true) {
        int remSum = sum;
        seq_t cur = 0;
        bool ok = true;

        for (int i = 0; i < len; ++i) {
            int remPos = len - i - 1;

            if (i == len - 1) {            /* last digit: must be even. */
                int d = remSum;
                if (d > 9 || (d & 1)) {   /* not a valid last digit */
                    ok = false;
                    break;
                }
                cur = cur * 10 + (seq_t)d;
                remSum = 0;
            } else {
                int start = (i == 0 && len > 1) ? 1 : 0;
                bool found = false;
                for (int d = start; d <= 9; ++d) {
                    if (d > remSum) break;
                    int nextRem = remSum - d;
                    if (feasible(remPos, nextRem)) {
                        cur = cur * 10 + (seq_t)d;
                        remSum -= d;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    ok = false;
                    break;
                }
            }
        }

        if (ok && remSum == 0) {
            M = cur;
            break;                       /* minimal number found */
        }
        ++len;                            /* try with one more digit */
    }

    return M / 2;                         /* k = M / 2 */
}
