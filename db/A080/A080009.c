/*
 * A080009  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=2, r=3, I={2}.
 * https://oeis.org/A080009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-1)+a(n-2)+2*a(n-3)+3*a(n-4)+5*a(n-5)+a(n-7)-a(n-8)-a(n-9)-a(n-10)
 *            Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=11,
 *            a(5)=26, a(6)=56, a(7)=127, a(8)=288, a(9)=660, a(10)=1500
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[] = {
    (seq_t)1,
    (seq_t)1,
    (seq_t)2,
    (seq_t)4,
    (seq_t)11,
    (seq_t)26,
    (seq_t)56,
    (seq_t)127,
    (seq_t)288,
    (seq_t)660,
    (seq_t)1500
};

seq_t a(idx_t n)
{
    if (n <= 10) {
        return init_vals[n];
    }

    /* Allocate space for all required terms up to n. */
    seq_t *buf = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!buf) { /* allocation failure – unlikely in test environment */
        return (seq_t)0;
    }

    /* Copy known initial terms. */
    for (idx_t i = 0; i <= 10; ++i) {
        buf[i] = init_vals[i];
    }

    /* Compute the sequence via the recurrence. */
    for (idx_t i = 11; i <= n; ++i) {
        buf[i] =
            buf[i - 1] + buf[i - 2]
            + (seq_t)2 * buf[i - 3]
            + (seq_t)3 * buf[i - 4]
            + (seq_t)5 * buf[i - 5]
            + buf[i - 7]
            - buf[i - 8]
            - buf[i - 9]
            - buf[i -10];
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
