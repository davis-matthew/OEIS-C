/*
 * A032974  Numbers with the property that all pairs of consecutive base-3
 *          digits differ by 0 or 1.
 * https://oeis.org/A032974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers whose consecutive base‑3 digits differ by 0 or 1.
   The n‑th term (for n>=1) is the n‑th smallest integer with that
   property, in increasing order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n should be >=1; for n==0 the value is undefined. */
    if (n == 0)
        return 0;

    seq_t n_seq = (seq_t)n;          /* use seq_t for all arithmetic */
    seq_t cumulative = 0;            /* numbers of all shorter lengths   */
    int L = 1;                       /* current length being examined   */

    /* dp[rem][d] will be filled later; for length 1 we need only the
       counts of leading digits 1 and 2. */
    seq_t f0 = 1, f1 = 1, f2 = 1;     /* dp[0][0], dp[0][1], dp[0][2]  */
    seq_t count_len = 2;              /* for L==1 (digits 1 or 2)       */

    /* Find the length L such that n lies within numbers of that length. */
    while (cumulative + count_len < n_seq) {
        cumulative += count_len;
        /* build dp for the next remainder length (rem = L) */
        seq_t nf0 = f0 + f1;
        seq_t nf1 = f0 + f1 + f2;
        seq_t nf2 = f1 + f2;
        f0 = nf0; f1 = nf1; f2 = nf2;
        ++L;
        /* for L>=2 the first digit cannot be 0: count = dp[rem][1]+dp[rem][2] */
        count_len = f1 + f2;
    }

    /* Offset of our target within this length (1‑based). */
    seq_t offset = n_seq - cumulative;

    /* Build DP table for all remaining lengths up to L-1. */
    /* Maximum L for n<=10000 is < 20; use static array size 25. */
    seq_t dp[25][3];
    for (int d = 0; d < 3; ++d)
        dp[0][d] = 1;                /* empty suffix */

    for (int rem = 1; rem < L; ++rem) {
        dp[rem][0] = dp[rem - 1][0] + dp[rem - 1][1];
        dp[rem][1] = dp[rem - 1][0] + dp[rem - 1][1] + dp[rem - 1][2];
        dp[rem][2] = dp[rem - 1][1] + dp[rem - 1][2];
    }

    /* Store the digits (base‑3) of the answer. */
    uint8_t digits[25];
    int first_digit = -1;
    /* choose first digit: 1 or 2 */
    for (int d = 1; d <= 2; ++d) {
        seq_t cnt = dp[L - 1][d];
        if (offset <= cnt) {
            first_digit = d;
            break;
        }
        offset -= cnt;
    }
    digits[0] = (uint8_t)first_digit;
    int last = first_digit;

    /* Determine the remaining digits. */
    for (int pos = 1; pos < L; ++pos) {
        int rem = L - pos - 1;      /* digits still to place after this one */
        for (int nd = 0; nd <= 2; ++nd) {
            if ( (nd >= last - 1) && (nd <= last + 1) ) { /* diff <=1 */
                seq_t cnt = dp[rem][nd];
                if (offset <= cnt) {
                    digits[pos] = (uint8_t)nd;
                    last = nd;
                    break;
                }
                offset -= cnt;
            }
        }
    }

    /* Convert the base‑3 digits to a base‑10 number. */
    seq_t value = 0;
    for (int i = 0; i < L; ++i) {
        value = value * 3 + digits[i];
    }

    return value;
}
