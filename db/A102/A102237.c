/*
 * A102237  Smallest number equal to the product of n primes which is also
 *          equal to the sum of n distinct primes.
 * https://oeis.org/A102237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n for n>5; a(1)=2, a(2)=9, a(3)=12, a(4)=36, a(5)=48
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)36;
    if (n == 5) return (seq_t)48;

    /* For n > 5 the conjectured value is 2^n.  Compute it by repeated
       multiplication by 2, which works for any seq_t (including
       arbitrary–precision integer classes).  */
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        res *= (seq_t)2;
    }
    return res;
}
