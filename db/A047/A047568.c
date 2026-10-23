/*
 * A047568  Numbers that are congruent to {1, 4, 5, 6, 7} mod 8.
 * https://oeis.org/A047568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5)+8 for n>5, with a(1)=1,a(2)=4,a(3)=5,a(4)=6,a(5)=7
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  n-1 = 5*k + r  (k >= 0, 0 <= r < 5) */
    idx_t k = (n - 1) / 5;
    idx_t r = (n - 1) % 5;

    /* base = 8*k, added offset depends on r */
    static const seq_t offsets[5] = { (seq_t)1, (seq_t)4,
                                      (seq_t)5, (seq_t)6,
                                      (seq_t)7 };
    seq_t base = (seq_t)k * 8;
    return base + offsets[r];
}
