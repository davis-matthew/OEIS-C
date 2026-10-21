/*
 * A061504  a(1) = 1; for n>1, a(n) = numbers of letters in French name for
 *          a(n-1).
 * https://oeis.org/A061504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(n) = a(n-4) for n > 6, with initial values a(1)=1, a(2)=2,
 *            a(3)=4, a(4)=6, a(5)=3, a(6)=5
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first six terms */
    static const seq_t init[6] = { (seq_t)1, (seq_t)2,
                                   (seq_t)4, (seq_t)6,
                                   (seq_t)3, (seq_t)5 };

    if (n <= 2)                /* a(1) and a(2) */
        return init[n - 1];

    /* for n >= 3 the sequence is periodic with period 4:
       indices 3,4,5,6 repeat thereafter.  */
    idx_t m = (n - 3) % 4;   /* m ∈ {0,1,2,3} */
    return init[2 + m];
}
