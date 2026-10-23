/*
 * A159885  For n >= 1, let f(2n+1) = (3n+2)/A006519(3n+2) and let f^k be the
 *          k-th iteration of f. Then a(n) is the least k such that
 *          A000120(f^k(2n+1)) <= A000120(n).
 * https://oeis.org/A159885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = minimal k such that popcount(f^k(2n+1)) <= popcount(n)
 *            where f(odd m) = odd_part((3*m+1)/2) = ((3*m+1)/2) /
 *            lsb((3*m+1)/2) (lsb = least significant set bit, i.e. highest
 *            power of 2 dividing the number)
 * Range:     u8 n <= 19, u16 n <= 8550, u32 n <= 8551, u64 n <= 8551,
 *            u128 n <= 8551, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int popcnt_seq_t(seq_t x)
{
    int c = 0;
    while (x) {
        x &= x - 1; /* drop lowest set bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* 1. initial odd term: 2n+1  */
    seq_t cur = (seq_t)n * 2 + 1;

    /* 2. popcount of the index */
    int pc_n = popcnt_seq_t((seq_t)n);

    int k = 0;
    while (popcnt_seq_t(cur) > pc_n) {
        ++k;
        /* compute next term: ((3*cur+1)/2) / lsb((3*cur+1)/2)  */
        seq_t x = (cur * 3 + 1) / 2;
        seq_t lsb = x & (~x + 1);          /* smallest power of two dividing x */
        seq_t next = x / lsb;
        cur = next;
    }
    return (seq_t)k;
}
