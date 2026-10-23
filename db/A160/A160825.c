/*
 * A160825  a(1) = 1. For n >=2, a(n) = the smallest integer > a(n-1) such
 *          that both a(n) and a(n)-a(n-1) have the same number of
 *          (non-leading) 0's when they are represented in binary.
 * https://oeis.org/A160825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 60, u64 n <= 124,
 *            u128 n <= 252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A160825:  a(1)=1; a(2)=6; a(3)=7; a(4)=13; a(5)=22;
   For n>5: a(n) = 2 * a(n-2)  (doubling step).
   Closed form:
      if n is even:   a(n) = 13 << ((n-4)/2);
      if n is odd:    a(n) = 22 << ((n-5)/2);    */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)7;
    if (n == 4) return (seq_t)13;
    if (n == 5) return (seq_t)22;
    if ((n & 1) == 0) {           /* even index */
        idx_t k = (n - 4) / 2;
        return ((seq_t)13 << k);
    } else {                      /* odd index */
        idx_t k = (n - 5) / 2;
        return ((seq_t)22 << k);
    }
}
