/*
 * A220398  A modified Engel expansion of the golden ratio (1/2)*(1 + sqrt(5))
 *          (A001622).
 * https://oeis.org/A220398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1,a(2)=2,a(3)=5,a(4)=8,a(5)=3,a(6)=4,a(7)=4,a(8)=6,a(9)=2,
 *            a(10)=162,a(11)=322,a(12)=2. For n≥13: if n%3==0 → a(n)=2; if
 *            n%3==1 → a(n)=2*(a(n-3)-1)²; if n%3==2 → a(n)=2*a(n-1)-2;
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 15, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n<1 */

    /* initial values, n=1..12 */
    static const seq_t init[12] = {
        1, 2, 5, 8, 3, 4, 4, 6, 2, 162, 322, 2
    };

    if (n <= 12) return init[n - 1];

    /* sliding window: a[n-3], a[n-2], a[n-1] */
    seq_t a_nm3 = init[9];   /* a(10) */
    seq_t a_nm2 = init[10];  /* a(11) */
    seq_t a_nm1 = init[11];  /* a(12) */

    seq_t cur = 0;

    for (idx_t i = 13; i <= n; ++i) {
        if ((i % 3) == 0) {
            cur = 2;
        } else if ((i % 3) == 1) {
            seq_t t = a_nm3;
            t = t - 1;
            cur = (t * t) * 2;
        } else { /* i%3 == 2 */
            cur = (a_nm1 * 2) - 2;
        }

        /* shift the window for the next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return cur;
}
