/*
 * A036112  A summarize Fibonacci sequence starting with a(0)=3 and a(1)=1:
 *          summarize the previous two terms!.
 * https://oeis.org/A036112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   append_int, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 3, a(1) = 1.
   For n ≥ 2, a(n) is formed from a(n‑1) and a(n‑2) by:
   • Count how many times each decimal digit (0…9) occurs in both numbers.
   • For each digit d with a non‑zero count, append the decimal
     representation of that count followed by the digit d.
   • The digit‑pairs are appended in order of decreasing digit. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"
#include "../utils/append_int.h"

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0)
        return (seq_t)3;
    if (n == 1)
        return (seq_t)1;

    /* Buffers for the last two terms */
    char *term[2];
    size_t len[2];
    size_t cap[2];

    term[0] = (char *)malloc(2);
    term[0][0] = '3';
    term[0][1] = '\0';
    len[0] = 1;
    cap[0] = 2;

    term[1] = (char *)malloc(2);
    term[1][0] = '1';
    term[1][1] = '\0';
    len[1] = 1;
    cap[1] = 2;

    for (idx_t i = 2; i <= n; ++i) {
        size_t cur = (size_t)(i & 1ULL);
        size_t p1  = (size_t)((i - 1) & 1ULL);
        size_t p2  = (size_t)((i - 2) & 1ULL);

        int cnt[10] = {0};

        /* Count digits in the two previous terms */
        for (size_t k = 0; k < len[p1]; ++k)
            ++cnt[(unsigned char)term[p1][k] - '0'];
        for (size_t k = 0; k < len[p2]; ++k)
            ++cnt[(unsigned char)term[p2][k] - '0'];

        /* Estimate required buffer length */
        size_t need = 0;
        for (int d = 9; d >= 0; --d) {
            if (cnt[d]) {
                int temp = cnt[d];
                int diglen = 0;
                do {
                    ++diglen;
                    temp /= 10;
                } while (temp > 0);
                need += diglen + 1; /* count digits + the digit itself */
            }
        }

        /* Resize buffer if necessary */
        if (cap[cur] < need) {
            size_t newcap = cap[cur] ? cap[cur] : 1;
            while (newcap < need)
                newcap <<= 1;
            char *newptr = (char *)realloc(term[cur], newcap);
            term[cur] = newptr;
            cap[cur] = newcap;
        }

        /* Build the new term */
        size_t pos = 0;
        for (int d = 9; d >= 0; --d) {
            int c = cnt[d];
            if (c == 0)
                continue;
            append_int(term[cur], &pos, c);
            term[cur][pos++] = '0' + d;
        }
        term[cur][pos] = '\0';
        len[cur] = pos;
    }

    /* Convert final term string to seq_t */
    seq_t result = seq_from_str(term[n & 1ULL]);

    /* Free allocated buffers */
    free(term[0]);
    free(term[1]);

    return result;
}
