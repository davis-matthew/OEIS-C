/*
 * A026608  a(n) = number of 2's between n-th 1 and (n+1)st 1 in A026600.
 * https://oeis.org/A026608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2's between the n-th 1 and the (n+1)-th 1 of
 *            the fixed point of the 3-symbol Thue–Morse morphism μ(1)=123,
 *            μ(2)=231, μ(3)=312
 * Range:     u8 n <= 9203, u16 n <= 9203, u32 n <= 9203, u64 n <= 9203,
 *            u128 n <= 9203, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 */

    /* Find a length L such that  L/3  ≥ n+1  (i.e. L ≥ 3*(n+1)). */
    idx_t target = 3 * (n + 1);
    size_t L = 1;
    while ((idx_t)L < target) L *= 3;

    /* Allocate and build the word of length L by successive expansions. */
    unsigned char *seq = (unsigned char*)malloc(L);
    if (!seq) return 0;                    /* allocation failed */
    seq[0] = 1;                            /* start with the seed symbol 1 */
    size_t cur_len = 1;

    static const unsigned char MU[4][3] = {
        {0,0,0},
        {1,2,3},
        {2,3,1},
        {3,1,2}
    };

    while (cur_len < L) {
        size_t next_len = cur_len * 3;
        unsigned char *next = (unsigned char*)malloc(next_len);
        if (!next) { free(seq); return 0; }

        for (size_t i = 0; i < cur_len; ++i) {
            unsigned char v = seq[i];
            next[3*i]   = MU[v][0];
            next[3*i+1] = MU[v][1];
            next[3*i+2] = MU[v][2];
        }
        free(seq);
        seq     = next;
        cur_len = next_len;
    }

    /* Scan the word to count 2's between the n-th and (n+1)-st 1. */
    seq_t count2 = 0;
    idx_t ones_seen = 0;
    int  counting  = 0;          /* 0 = not counting, 1 = counting 2's */

    for (size_t i = 0; i < L; ++i) {
        unsigned char c = seq[i];
        if (c == 1) {
            ++ones_seen;
            if (ones_seen == n) {
                counting = 1;     /* start counting after the n-th 1 */
                count2  = 0;
            } else if (ones_seen == n + 1) {
                break;            /* reached the (n+1)-st 1 */
            }
        } else if (counting && c == 2) {
            ++count2;
        }
    }

    free(seq);
    return count2;
}
