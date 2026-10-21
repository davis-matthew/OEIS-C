/*
 * A229654  Quadrisection a(4n+k) gives k-th differences of a for k=0..3 with
 *          a(n)=0 for n<3 and a(3)=1.
 * https://oeis.org/A229654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, eigen, look
 * Formula:   not stated
 * Range:     i8 n <= 338, i16 n <= 9636, i32 n <= 9636, i64 n <= 9636,
 *            i128 n <= 9636, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Quadrisection:
   a(0)=0, a(1)=0, a(2)=0, a(3)=1
   a(4n)   = a(n)
   a(4n+1) = a(n+1)-a(n)
   a(4n+2) = a(n+2)-2*a(n+1)+a(n)
   a(4n+3) = a(n+3)-3*a(n+2)+3*a(n+1)-a(n) */

seq_t a(idx_t n)
{
    /* Base values for n < 3 and n == 3 */
    if (n == 0 || n == 1 || n == 2)
        return 0;
    if (n == 3)
        return 1;

    /* Divide n by 4: q = n/4, r = n%4 */
    idx_t q = n >> 2;          /* integer division by 4 */
    int    r = n & 3;          /* remainder 0..3 */

    switch (r) {
        case 0:
            return a(q);

        case 1: {
            seq_t Aq1 = a(q + 1);
            seq_t Aq  = a(q);
            return Aq1 - Aq;
        }

        case 2: {
            seq_t Aq2 = a(q + 2);
            seq_t Aq1 = a(q + 1);
            seq_t Aq  = a(q);
            return Aq2 - (seq_t)2 * Aq1 + Aq;
        }

        case 3: {
            seq_t Aq3 = a(q + 3);
            seq_t Aq2 = a(q + 2);
            seq_t Aq1 = a(q + 1);
            seq_t Aq  = a(q);
            return Aq3 - (seq_t)3 * Aq2 + (seq_t)3 * Aq1 - Aq;
        }
    }

    /* Unreachable, but keep the compiler happy */
    return 0;
}
