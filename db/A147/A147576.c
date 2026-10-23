/*
 * A147576  Numbers with exactly 3 distinct odd prime divisors {3,5,7}.
 * https://oeis.org/A147576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 105 * A108347(n)
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 378, u64 n <= 3595,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A108347(n) is the nth number of the form 3^i 5^j 7^k (i,j,k≥0) in increasing order. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n < 1 */

    static seq_t *b = NULL;                /* array of A108347 values */
    static idx_t cur_len = 0;              /* number of elements currently computed */
    static idx_t cap = 0;                  /* allocated capacity of b */
    static idx_t i3 = 0, i5 = 0, i7 = 0;   /* indices for next multiples of 3,5,7 */

    /* grow the array if necessary */
    if (cap < n) {
        seq_t *tmp = (seq_t *)realloc(b, (size_t)n * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        b = tmp;
        cap = n;
    }

    /* initialise on first call */
    if (cur_len == 0) {
        b[0] = (seq_t)1;
        cur_len = 1;
        i3 = i5 = i7 = 0;
    }

    /* generate A108347 values up to index n */
    while (cur_len < n) {
        seq_t t3 = (seq_t)3 * b[i3];
        seq_t t5 = (seq_t)5 * b[i5];
        seq_t t7 = (seq_t)7 * b[i7];
        seq_t nxt = t3;

        if (t5 < nxt) nxt = t5;
        if (t7 < nxt) nxt = t7;

        b[cur_len++] = nxt;

        if (t3 == nxt) ++i3;
        if (t5 == nxt) ++i5;
        if (t7 == nxt) ++i7;
    }

    /* a(n) = 105 * A108347(n) */
    return (seq_t)105 * b[n - 1];
}
