/*
 * A195658  Number of ways to place 9n nonattacking kings on a vertical
 *          cylinder 18 X 2n.
 * https://oeis.org/A195658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -4*a(n-10) + 36*a(n-9) - 145*a(n-8) + 344*a(n-7) * -
 *            532*a(n-6) + 560*a(n-5) - 406*a(n-4) + 200*a(n-3) * - 64*a(n-2)
 *            + 12*a(n-1) * Initial values: * a(1)=1024, a(2)=50922,
 *            a(3)=815816, a(4)=7238864, * a(5)=44693472, a(6)=216134044,
 *            a(7)=877751236, * a(8)=3130270224, a(9)=10105541204, a(10)...
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 35, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static const seq_t init[10] = {
    (seq_t)1024ULL,
    (seq_t)50922ULL,
    (seq_t)815816ULL,
    (seq_t)7238864ULL,
    (seq_t)44693472ULL,
    (seq_t)216134044ULL,
    (seq_t)877751236ULL,
    (seq_t)3130270224ULL,
    (seq_t)10105541204ULL,
    (seq_t)30179587994ULL
};

seq_t a(idx_t n)
{
    if (n <= (idx_t)10)
        return init[(size_t)(n - 1)];

    /* Buffer holding the last 10 computed terms:
       buf[0] = a(1), buf[1] = a(2), … , buf[9] = a(10)   */
    seq_t buf[10];
    for (size_t i = 0; i < 10; ++i)
        buf[i] = init[i];

    for (idx_t k = 11; k <= n; ++k) {
        seq_t pos = (seq_t)12  * buf[9]   /* a(k-1) */
                 + (seq_t)200 * buf[7]   /* a(k-3) */
                 + (seq_t)560 * buf[5]   /* a(k-5) */
                 + (seq_t)344 * buf[3]   /* a(k-7) */
                 + (seq_t)36  * buf[1];  /* a(k-9) */

        seq_t neg = (seq_t)64  * buf[8]   /* a(k-2) */
                 + (seq_t)406 * buf[6]   /* a(k-4) */
                 + (seq_t)532 * buf[4]   /* a(k-6) */
                 + (seq_t)145 * buf[2]   /* a(k-8) */
                 + (seq_t)4   * buf[0];  /* a(k-10) */

        seq_t next = pos - neg;          /* a(k) */

        for (size_t i = 0; i < 9; ++i)
            buf[i] = buf[i + 1];
        buf[9] = next;
    }

    return buf[9];
}
