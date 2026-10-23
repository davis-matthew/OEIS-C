/*
 * A110935  a(n) = if n mod 2 = 0 then 8*F(n)-n otherwise 8*F(n)-4, where F()
 *          = Fibonacci numbers A000045.
 * https://oeis.org/A110935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n mod 2 = 0 then 8*F(n) - n else 8*F(n) - 4, where F
 *            is the Fibonacci sequence A000045 (F(0)=0, F(1)=1)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute Fibonacci F(n) iteratively */
    seq_t f0 = 0;    /* F(0) */
    seq_t f1 = 1;    /* F(1) */
    seq_t fn;

    if (n == 0) {
        fn = f0;
    } else if (n == 1) {
        fn = f1;
    } else {
        idx_t i;
        for (i = 2; i <= n; ++i) {
            seq_t tmp = f0 + f1;
            f0 = f1;
            f1 = tmp;
        }
        fn = f1;
    }

    seq_t res = fn * (seq_t)8;
    if ((n & 1) == 0) {
        res -= (seq_t)n;   /* even n */
    } else {
        res -= (seq_t)4;   /* odd n */
    }

    return res;
}
