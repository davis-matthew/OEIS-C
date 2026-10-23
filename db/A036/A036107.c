/*
 * A036107  A summarize Fibonacci sequence starting with a(0)=1 and a(1)=4:
 *          summarize the previous two terms!.
 * https://oeis.org/A036107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(0)=1, a(1)=4. For n ≥ 2, a(n) is obtained by summarizing the
 *            previous two terms: Count the total occurrences of each decimal
 *            digit in a(n‑1) and a(n‑2), list the digits in descending order,
 *            and for each digit d output the number of occurrences followed
 *            by the digit itself. The resulting concate...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   string_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/string_to_seq.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* handle the two initial terms directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    /* strings holding the last two terms */
    char *s0 = (char *)malloc(2);
    char *s1 = (char *)malloc(2);
    if (!s0 || !s1) {
        free(s0); free(s1);
        return (seq_t)0;
    }
    s0[0] = '1'; s0[1] = '\0';
    s1[0] = '4'; s1[1] = '\0';

    for (idx_t i = 2; i <= n; ++i) {
        /* count digits in the two previous terms */
        int cnt[10] = {0};
        for (char *p = s0; *p; ++p)
            ++cnt[*p - '0'];
        for (char *p = s1; *p; ++p)
            ++cnt[*p - '0'];

        /* compute required buffer size */
        size_t needed = 1; /* for the terminating NUL */
        for (int d = 9; d >= 0; --d) {
            int c = cnt[d];
            if (c == 0)
                continue;
            int digits = 0;
            int tmp = c;
            do {
                ++digits;
                tmp /= 10;
            } while (tmp);
            needed += digits + 1; /* count digits + the digit itself */
        }

        char *s2 = (char *)malloc(needed);
        if (!s2) {
            free(s0); free(s1);
            return (seq_t)0;
        }

        /* build the new term */
        size_t pos = 0;
        for (int d = 9; d >= 0; --d) {
            int c = cnt[d];
            if (c == 0)
                continue;
            /* convert count to string */
            int tmp = c;
            char rev[20];
            int revlen = 0;
            do {
                rev[revlen++] = '0' + (tmp % 10);
                tmp /= 10;
            } while (tmp);
            for (int k = revlen - 1; k >= 0; --k)
                s2[pos++] = rev[k];
            /* append the digit itself */
            s2[pos++] = (char)('0' + d);
        }
        s2[pos] = '\0';

        /* rotate buffers for next iteration */
        free(s0);
        s0 = s1;
        s1 = s2;
    }

    /* convert final string to seq_t and clean up */
    seq_t result = string_to_seq(s1);
    free(s0);
    free(s1);
    return result;
}
