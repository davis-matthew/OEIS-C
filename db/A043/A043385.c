/*
 * A043385  Numbers having one 4 in base 6.
 * https://oeis.org/A043385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose base‑6 representation
 *            contains exactly one digit '4'
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Offset is 1, but behaviour for n==0 is unspecified */
    if (n < (idx_t)1) return (seq_t)0;

    /* Length 1 is trivial */
    if (n == (idx_t)1) return (seq_t)4;

    /* Pre‑compute powers of 5.  5^128 is astronomically large, more than
       enough for any n we are expected to handle. */
    enum { MAX_L = 128 };
    static seq_t pow5[MAX_L];
    static bool pow5_inited = false;
    if (!pow5_inited) {
        pow5[0] = (seq_t)1;
        for (int i = 1; i < MAX_L; ++i)
            pow5[i] = pow5[i-1] * (seq_t)5;
        pow5_inited = true;
    }

    /* Find the length L of the desired number */
    seq_t cumulative = (seq_t)0;
    int L = 1;
    seq_t countL;
    seq_t n_seq = (seq_t)n;
    while (1) {
        if (L == 1) {
            countL = (seq_t)1;                    /* only the number 4 */
        } else {
            seq_t term1 = pow5[L-1];              /* leading digit = 4 */
            seq_t term2 = (seq_t)(4*(L-1)) * pow5[L-2];
            countL = term1 + term2;               /* leading digit ≠ 4 */
        }
        if (cumulative + countL >= n_seq) break;
        cumulative += countL;
        ++L;
    }

    /* Offset within the block of length L numbers (0‑based) */
    seq_t offset = n_seq - cumulative - (seq_t)1;

    /* Determine the leading digit.  Digits are tried in increasing order:
       1,2,3,4,5.  (4 is placed between 3 and 5 in order.) */
    int used4 = 0;
    int leading = 0;
    for (int d = 1; d <= 5; ++d) {
        seq_t cnt;
        if (d == 4)
            cnt = pow5[L-1];
        else
            cnt = (seq_t)(L-1) * pow5[L-2];
        if (offset < cnt) {
            leading = d;
            used4   = (d == 4);
            break;
        } else {
            offset -= cnt;
        }
    }

    seq_t value = (seq_t)leading;
    int remaining = L - 1;            /* positions after the leading digit */

    /* Construct the remaining digits from left to right.  For each
       position we try digits 0,1,2,3,4,5 in numeric order, skipping
       digit 4 if it has already been used.  For each candidate we
       count how many completions are possible; if the offset is smaller
       than that count we pick the digit, otherwise we subtract that
       count and continue. */
    while (remaining > 0) {
        for (int d = 0; d <= 5; ++d) {
            if (d == 4 && used4) continue;

            bool next_used4 = used4 || (d == 4);
            int rem_after   = remaining - 1;
            seq_t cnt;

            if (!next_used4) {
                /* Have not used 4 yet: must place it somewhere in the
                   remaining positions. */
                if (rem_after == 0)
                    cnt = (seq_t)0;               /* impossible */
                else
                    cnt = (seq_t)rem_after * pow5[rem_after-1];
            } else {
                /* 4 already used: all remaining digits must avoid 4 */
                cnt = pow5[rem_after];
            }

            if (offset < cnt) {
                value = value * (seq_t)6 + (seq_t)d;
                used4 = next_used4;
                remaining = rem_after;
                break;
            } else {
                offset -= cnt;
            }
        }
    }

    return value;
}
