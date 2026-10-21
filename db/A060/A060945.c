/*
 * A060945  Number of compositions (ordered partitions) of n into 1's, 2's and
 *          4's.
 * https://oeis.org/A060945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-4); a(0)=1, a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 79, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *buf = NULL;
    static idx_t buf_sz = 0;   /* number of allocated slots */
    static idx_t computed_upto = 0; /* highest index that has been computed */

    /* helper to grow the buffer up to >= new_sz */
    if (buf_sz == 0) {
        /* start with room for at least 4 entries */
        buf_sz = 4;
        buf = (seq_t *)malloc(buf_sz * sizeof(seq_t));
        if (!buf) return (seq_t)0; /* allocation failure protection */
        /* initialise base values */
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)1;
        buf[2] = (seq_t)2;
        buf[3] = (seq_t)3;
        computed_upto = 3;
    }
    else if (n + 1 > buf_sz) {
        /* grow buffer to accommodate index n */
        idx_t new_sz = n + 1;
        if (new_sz < buf_sz * 2) new_sz = buf_sz * 2;
        seq_t *tmp = (seq_t *)realloc(buf, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure protection */
        buf = tmp;
        buf_sz = new_sz;
    }

    /* compute any missing values up to n */
    if (n > computed_upto) {
        idx_t i;
        for (i = computed_upto + 1; i <= n; ++i) {
            if (i < 4) {
                /* base values already set */
                continue;
            }
            seq_t sum = buf[i - 1];
            sum += buf[i - 2];
            sum += buf[i - 4];
            buf[i] = sum;
        }
        computed_upto = n;
    }

    /* n is now within computed range */
    return buf[n];
}
