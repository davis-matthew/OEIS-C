/*
 * A211531  Number of -1..1 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having one,
 *          two, three, four, five or six distinct values for every i,j,k<=n.
 * https://oeis.org/A211531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=9, a(2)=27, a(3)=77, a(4)=199, a(5)=503, a(6)=1239,
 *            a(7)=3021, a(8)=7303, a(9)=17583
 *            a(n)=5*a(n-1)-3*a(n-2)-17*a(n-3)+17*a(n-4)+22*a(n-5)
 *            -18*a(n-6)-14*a(n-7)+3*a(n-8)+2*a(n-9) for n>9
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[9] = {
        (seq_t)9,  (seq_t)27,  (seq_t)77,  (seq_t)199,
        (seq_t)503, (seq_t)1239, (seq_t)3021, (seq_t)7303,
        (seq_t)17583
    };

    if (n <= 9u) {
        return init[(size_t)(n - 1u)];
    }

    /* Memoisation buffer */
    static seq_t *buf = NULL;
    static size_t buf_cap = 0;
    static size_t buf_used = 0;

    if (!buf) {
        buf_cap = 9u;
        buf = (seq_t *)malloc(buf_cap * sizeof(seq_t));
        for (size_t i = 0; i < 9u; ++i) buf[i] = init[i];
        buf_used = 9u;
    }

    if ((size_t)n > buf_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure */
        buf = tmp;
        buf_cap = new_cap;
    }

    for (size_t i = buf_used; i < (size_t)n; ++i) {
        seq_t t1 = buf[i - 1];
        seq_t t2 = buf[i - 2];
        seq_t t3 = buf[i - 3];
        seq_t t4 = buf[i - 4];
        seq_t t5 = buf[i - 5];
        seq_t t6 = buf[i - 6];
        seq_t t7 = buf[i - 7];
        seq_t t8 = buf[i - 8];
        seq_t t9 = buf[i - 9];

        seq_t val = (seq_t)5 * t1 + (seq_t)17 * t4 + (seq_t)22 * t5
                    + (seq_t)3 * t8 + (seq_t)2 * t9
                    - ((seq_t)3 * t2 + (seq_t)17 * t3
                       + (seq_t)18 * t6 + (seq_t)14 * t7);

        buf[i] = val;
    }

    buf_used = (size_t)n;
    return buf[(size_t)(n - 1u)];
}
