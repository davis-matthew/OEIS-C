/*
 * A177423  a(n) = 6*a(n-1)-8*a(n-2)-3 for n > 10; a(0)=753, a(1)=5043,
 *          a(2)=25779, a(3)=213003, a(4)=1687623, a(5)=18171219,
 *          a(6)=73344977, a(7)=769993485, a(8)=4480470831, a(9)=17582325855,
 *          a(10)=69524390079.
 * https://oeis.org/A177423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 23, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq: A177423
 * a(n) = 6*a(n-1)-8*a(n-2)-3   (n > 10)
 * a(0)=753, a(1)=5043, a(2)=25779, a(3)=213003, a(4)=1687623,
 * a(5)=18171219, a(6)=73344977, a(7)=769993485, a(8)=4480470831,
 * a(9)=17582325855, a(10)=69524390079
 */

#include <stddef.h>

static const seq_t base[11] = {
    (seq_t)753ULL,
    (seq_t)5043ULL,
    (seq_t)25779ULL,
    (seq_t)213003ULL,
    (seq_t)1687623ULL,
    (seq_t)18171219ULL,
    (seq_t)73344977ULL,
    (seq_t)769993485ULL,
    (seq_t)4480470831ULL,
    (seq_t)17582325855ULL,
    (seq_t)69524390079ULL
};

seq_t a(idx_t n)
{
    if (n <= 10)
        return base[n];

    seq_t a_nm1 = base[10];   /* a(n-1) when i==11 */
    seq_t a_nm2 = base[9];    /* a(n-2) when i==11 */

    for (idx_t i = 11; i <= n; ++i) {
        seq_t next = (seq_t)6 * a_nm1 - (seq_t)8 * a_nm2 - (seq_t)3;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
