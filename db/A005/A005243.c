/*
 * A005243  A self-generating sequence: start with 1 and 2, take all sums of
 *          any number of successive previous elements and adjoin them to the
 *          sequence. Repeat!
 * https://oeis.org/A005243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   sieve - a(1)=1, a(2)=2; v > a(k) is a term iff v is a sum of >=
 *            2 consecutive earlier terms; each new term marks every run sum
 *            ending at it
 * Range:     u8 n <= 189, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static long long *A005243_terms = NULL;
static idx_t A005243_cnt = 0;

static void A005243_build(idx_t n) {
    if (n < 2) n = 2;
    long long L = 2 * (long long)n + 100;
    for (;;) {
        unsigned char *mark = (unsigned char *)calloc((size_t)(L + 1), 1);
        long long *t = (long long *)malloc(sizeof(long long) * (size_t)(n + 2));
        idx_t c = 0;
        t[c++] = 1;
        t[c++] = 2;
        /* runs ending at index 1 */
        {
            long long s = t[1];
            for (idx_t j = 1; j-- > 0;) {
                s += t[j];
                if (s > L) break;
                mark[s] = 1;
            }
        }
        long long v = t[c - 1] + 1;
        while (c < n && v <= L) {
            if (mark[v]) {
                t[c] = v;
                long long s = v;
                for (idx_t j = c; j-- > 0;) {
                    s += t[j];
                    if (s > L) break;
                    mark[s] = 1;
                }
                c++;
            }
            v++;
        }
        free(mark);
        if (c >= n) {
            free(A005243_terms);
            A005243_terms = t;
            A005243_cnt = c;
            return;
        }
        free(t);
        L *= 2;
    }
}

seq_t a(idx_t n) {
    if (n > A005243_cnt) {
        idx_t want = n < 64 ? 64 : n;
        if (want < 2 * A005243_cnt) want = 2 * A005243_cnt;
        A005243_build(want);
    }
    return (seq_t)(uint64_t)A005243_terms[n - 1];
}
