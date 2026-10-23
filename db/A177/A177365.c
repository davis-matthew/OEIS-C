/*
 * A177365  a(n) contains the nonzero frequencies f(d) of digits d=0 .. 9 in
 *          all terms up to a(n-1) in concatenated form sorted with respect to
 *          d: f(0)//0//f(1)//1//...//f(9)//9. Initial term a(1)=6.
 * https://oeis.org/A177365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 44,
 *            big any n
 * Helpers:   digit_counts, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A177365: a(1)=6.
   a(n) is formed by concatenating the non‑zero digit frequencies
   of all terms up to a(n‑1), in increasing digit order.  Each
   frequency f(d) followed by the digit d. */

/*------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_counts.h"
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    unsigned long long freq[10] = {0};
    unsigned char cnt[10];
    seq_t term = seq_from_str("6");

    /* initialise frequencies with the first term */
    digit_counts(term, cnt);
    for (int d = 0; d < 10; ++d)
        freq[d] = cnt[d];

    if (n == 1)
        return term;

    /* generate successive terms */
    for (idx_t i = 2; i <= n; ++i) {
        /* build a(i) from current frequencies */
        char buf[200] = {0};
        int pos = 0;
        for (int d = 0; d < 10; ++d) {
            if (freq[d] == 0)
                continue;
            pos += snprintf(buf + pos,
                            sizeof(buf) - pos,
                            "%llu%c",
                            freq[d],
                            '0' + d);
        }
        seq_t next = seq_from_str(buf);

        if (i == n)
            return next;

        /* update frequencies with digits of the new term */
        digit_counts(next, cnt);
        for (int d = 0; d < 10; ++d)
            freq[d] += cnt[d];
    }

    /* unreachable, but satisfies the compiler */
    return (seq_t)0;
}
