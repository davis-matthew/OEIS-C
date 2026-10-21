/*
 * A053397  The full list of 6-Kaprekar numbers.
 * https://oeis.org/A053397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) is the nth 6‑Kaprekar number. A number N is a 6‑Kaprekar
 *            number if floor(N^2 / 10^6) + N^2 % 10^6 = N. The sequence is
 *            finite; we generate all such numbers in the range 1 ≤ N ≤
 *            1,000,000 and return the nth term (a(1) = first term, a(2) =
 *            second term, …)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define LIMIT 1000000          /* upper bound for 6‑Kaprekar numbers */

seq_t a(idx_t n)
{
    /* cache of found numbers */
    static seq_t *list = NULL;
    static idx_t len  = 0;     /* number of terms stored */
    static idx_t cap  = 0;     /* allocated capacity */

    if (n == 0) return 0;      /* offset starts at 1 */

    /* initialise cache on first call */
    if (!list) {
        cap = 64;
        list = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!list) return 0;   /* allocation failure */

        const seq_t BASE = 1000000;   /* 10^6 */

        for (idx_t num = 1; num <= LIMIT; ++num) {
            seq_t sq = (seq_t)num * (seq_t)num;
            seq_t left = sq / BASE;
            seq_t right = sq % BASE;
            if (left + right == (seq_t)num) {
                if (len >= cap) {
                    cap <<= 1;           /* double capacity */
                    seq_t *tmp = (seq_t *)realloc(list, cap * sizeof(seq_t));
                    if (!tmp) return 0;  /* allocation failure */
                    list = tmp;
                }
                list[len++] = (seq_t)num;
            }
        }
    }

    if (n > len) return 0;    /* n beyond known terms */
    return list[n - 1];
}
