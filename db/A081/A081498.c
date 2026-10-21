/*
 * A081498  Consider the triangle in which the n-th row starts with n,
 *          contains n terms and the difference of successive terms is
 *          1,2,3,... up to n-1. Sequence gives row sums.
 * https://oeis.org/A081498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(1 + 6*n - n*n)/6 (OEIS A081498)
 * Range:     i8 n <= 11, i16 n <= 60, i32 n <= 2346, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 if ever called, though the sequence
       is defined only for n >= 1. */
    if (n == 0)
        return 0;

    /* Compute n*n safely in the seq_t type */
    seq_t nn = (seq_t)n;             /* n as seq_t */
    seq_t n2 = nn * nn;              /* n^2 */

    /* Compute (n-1)*n*(n+1)/6 without overflow by cancelling 2 and 3
       from the three consecutive numbers first. */
    seq_t p = nn;                  /* n      */
    seq_t q = (seq_t)(n - 1);      /* n-1    */
    seq_t r = (seq_t)(n + 1);      /* n+1    */

    /* Divide one of p, q, r by 2 (one of three consecutive numbers is even) */
    if ((p & 1) == 0) {
        p /= 2;
    } else if ((q & 1) == 0) {
        q /= 2;
    } else {
        r /= 2;
    }

    /* Divide one of p, q, r by 3 (one of three consecutive numbers is divisible by 3) */
    if ((p % 3) == 0) {
        p /= 3;
    } else if ((q % 3) == 0) {
        q /= 3;
    } else {
        r /= 3;
    }

    /* Now p*q*r equals (n-1)*n*(n+1)/6 exactly */
    seq_t bin = p * q * r;

    /* Final formula: a(n) = n^2 - binomial(n+1, 3) */
    return n2 - bin;
}
