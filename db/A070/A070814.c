/*
 * A070814  Solutions to phi(gpf(x)) - gpf(phi(x)) = 14 = c are special
 *          multiples of 17, x = 17k, where greatest prime factors of factor k
 *          were observed from {2, 3, 5}, i.e., it is smaller than 17. See
 *          solutions to other even cases of c (=A070813): A007283 for 0,
 *          A070004 for 2, A070815 for 254, A070816 for 6...
 * https://oeis.org/A070814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2 * a(n-4) for n > 10
   Initial values:
   a(1)=17, a(2)=34, a(3)=51, a(4)=68, a(5)=85,
   a(6)=102, a(7)=136, a(8)=170, a(9)=204, a(10)=255. */

seq_t a(idx_t n)
{
    /* base[0] is unused but keeps indices aligned with the offset. */
    static const seq_t base[11] = {
        0, 17, 34, 51, 68, 85, 102, 136, 170, 204, 255
    };

    if (n <= 10)
        return base[n];

    /* Reduce n by multiples of 4 until it lies in the base range. */
    idx_t t = (n - 11) / 4 + 1;          /* number of doublings */
    idx_t m = n - 4 * t;                 /* n after reductions (1..10) */

    seq_t res = base[m];
    for (idx_t i = 0; i < t; ++i)
        res = res * 2;                   /* multiply by 2 as many times as needed */

    return res;
}
