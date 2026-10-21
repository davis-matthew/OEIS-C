/*
 * A098154  Summarize the previous term in ternary (in increasing order).
 * https://oeis.org/A098154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is obtained by repeatedly summarizing the ternary *
 *            representation of the previous term (base‑3). Starting with
 *            a(1)=1, * a(n) is produced by counting the 0’s, 1’s, and 2’s in
 *            a(n‑1), * writing each count in base‑3 followed by the
 *            corresponding digit * (digits sorted 0,1,2), and concatenatin...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* base‑3 digits string of the current term */
    char cur[64] = "1";
    if (n == 1) {
        return (seq_t)1;
    }

    char next[64];
    for (idx_t step = 2; step <= n; ++step) {
        /* count occurrences of each ternary digit */
        int cnt[3] = {0, 0, 0};
        for (char *p = cur; *p; ++p)
            ++cnt[*p - '0'];

        /* build next term */
        int pos = 0;
        for (int d = 0; d <= 2; ++d) {
            if (cnt[d] == 0) continue;

            /* convert count to base‑3 (no leading zeros) */
            int val = cnt[d];
            char buf[16];
            int idx = 0;
            while (val > 0) {
                buf[idx++] = '0' + (val % 3);
                val /= 3;
            }
            /* val > 0 ensures idx>0 */
            for (int k = idx - 1; k >= 0; --k)
                next[pos++] = buf[k];
            /* append the digit itself */
            next[pos++] = '0' + d;
        }
        next[pos] = '\0';

        /* if we reached the fixed point, all further values are the same */
        if (strcmp(cur, next) == 0) {
            /* cur already equals next; we can break */
            break;
        }
        strcpy(cur, next);
    }

    /* convert decimal string to seq_t */
    seq_t result = (seq_t)0;
    for (char *p = cur; *p; ++p) {
        result = result * (seq_t)10 + (seq_t)(*p - '0');
    }
    return result;
}
