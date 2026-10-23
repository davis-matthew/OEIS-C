/*
 * A117606  a(n) = ones digit minus tens digit of the square of a(n-1), with
 *          a(0) = 2.
 * https://oeis.org/A117606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign, easy
 * Formula:   a(0)=2; a(n) = (a(n-1))² – its tens digit: a(n) = ones digit −
 *            tens digit of (a(n‑1))² For n>6 the sequence is periodic of
 *            period 4: a(n)=a(n‑4)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 7 terms computed directly (a(0) … a(6)) */
    static const seq_t init[7] = { (seq_t)2, (seq_t)4, (seq_t)5,
                                   (seq_t)3, (seq_t)9, (seq_t)(-7),
                                   (seq_t)5 };

    /* Reduce n until it falls into the pre‑computed range. */
    while (n > 6u)
        n -= 4u;

    return init[n];
}
