/*
 * A079959  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=1, r=5, I={2,4}.
 * https://oeis.org/A079959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-4)+a(n-6), a(0)=1, a(n)=0 for n<0
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *buf = NULL;
    static idx_t buf_sz = 0;      /* allocated size of buf */
    static idx_t last_computed = 0; /* highest n that has been evaluated */

    /* first call: allocate and initialise a(0)=1 */
    if (buf_sz == 0) {
        buf_sz = 1;
        buf = (seq_t *)malloc(buf_sz * sizeof(seq_t));
        if (!buf) return (seq_t)0; /* allocation failure: return zero */
        buf[0] = (seq_t)1;          /* a(0) = 1 */
        last_computed = 0;
    }

    /* enlarge buffer if needed */
    if (buf_sz < n + 1) {
        idx_t old_sz = buf_sz;
        buf_sz = (n + 1) * 2; /* grow by factor 2 for amortised cost */
        seq_t *tmp = (seq_t *)realloc(buf, buf_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        buf = tmp;
        /* initialise newly allocated slots to 0 */
        for (idx_t i = old_sz; i < buf_sz; ++i)
            buf[i] = (seq_t)0;
    }

    /* compute values up to n if not already done */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t sum = buf[i - 1];
            if (i >= 2) sum += buf[i - 2];
            if (i >= 4) sum += buf[i - 4];
            if (i >= 6) sum += buf[i - 6];
            buf[i] = sum;
        }
        last_computed = n;
    }

    return buf[n];
}
