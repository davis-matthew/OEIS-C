/*
 * A070608  Duplicate of A070486.
 * https://oeis.org/A070608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = a(n mod 24) where the base table is a(0)=0, a(1)=1,
 *            a(2)=8, a(3)=3, a(4)=16, a(5)=5, a(6)=0, a(7)=7, a(8)=8, a(9)=9,
 *            a(10)=16, a(11)=11, a(12)=0, a(13)=13, a(14)=8, a(15)=15,
 *            a(16)=16, a(17)=17, a(18)=0, a(19)=19, a(20)=8, a(21)=21,
 *            a(22)=16, a(23)=23
 * Range:     i8 n <= 28, i16 n <= 28, i32 n <= 28, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 24‑element periodic table */
    static const seq_t tbl[24] = {
        0, 1, 8, 3, 16, 5,
        0, 7, 8, 9, 16, 11,
        0, 13, 8, 15, 16, 17,
        0, 19, 8, 21, 16, 23
    };
    /*   idx_t is unsigned; n % 24 is safe and small. */
    idx_t r = n % 24;
    return tbl[r];
}
