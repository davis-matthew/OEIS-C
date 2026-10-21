/*
 * A237344  For k in {2,3,...,9} define a sequence as follows: a(0)=0; for
 *          n>=0, a(n+1)=a(n)+1, unless a(n) ends in k, in which case a(n+1)
 *          is obtained by replacing the last digit of a(n) with the digit(s)
 *          of k^2. This is k(7).
 * https://oeis.org/A237344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 79, u64 n <= 169,
 *            u128 n <= 340, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A237344 – k(7):  a(0)=0; for n>=0, if a(n) ends in 7 then
   a(n+1) = (a(n)/10)*100 + 49, else a(n+1)=a(n)+1. */

seq_t a(idx_t n)
{
    seq_t cur = 0;
    for (idx_t i = 0; i < n; ++i) {
        if ((cur % (seq_t)10) == (seq_t)7) {
            cur = (cur / (seq_t)10) * (seq_t)100 + (seq_t)49;
        } else {
            cur += (seq_t)1;
        }
    }
    return cur;
}
