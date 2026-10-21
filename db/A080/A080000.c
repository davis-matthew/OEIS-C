/*
 * A080000  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=2, r=3, I={-1,1,2}.
 * https://oeis.org/A080000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 35, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080000
   a(n) = a(n-1) + 2*a(n-5) - a(n-6) + a(n-7) - a(n-10)
   initial values:
   a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1,
   a(5)=2, a(6)=3, a(7)=5, a(8)=7, a(9)=9  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial 10 terms */
    static const seq_t init[10] = {1,1,1,1,1,2,3,5,7,9};

    /* cache for computed values */
    static seq_t *buf = NULL;
    static idx_t buf_len = 0;

    /* first call: allocate cache and copy initial values */
    if (buf_len == 0) {
        buf_len = 10;
        buf = (seq_t*)malloc(buf_len * sizeof(seq_t));
        if (!buf) abort();
        for (idx_t i = 0; i < buf_len; ++i)
            buf[i] = init[i];
    }

    /* extend cache if needed and compute new terms */
    if (n + 1 > buf_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(buf, new_len * sizeof(seq_t));
        if (!tmp) abort();
        buf = tmp;
        for (idx_t i = buf_len; i <= n; ++i) {
            seq_t val;
            if (i < 10) {
                val = init[i];
            } else {
                val = buf[i-1];          /* a(n-1) */
                val += buf[i-5];          /* + a(n-5) */
                val += buf[i-5];          /* + a(n-5) again */
                val -= buf[i-6];          /* - a(n-6) */
                val += buf[i-7];          /* + a(n-7) */
                val -= buf[i-10];         /* - a(n-10) */
            }
            buf[i] = val;
        }
        buf_len = new_len;
    }

    return buf[n];
}
