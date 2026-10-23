/*
 * A014991  a(n) = (1 - (-9)^n)/10.
 * https://oeis.org/A014991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1 - (-9)^n)/10
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 20, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence: a(1)=1, a(2)=-8,
    a(n) = -8*a(n-1) + 9*a(n-2)  for n>=3
    (valid for all n>=1; behaviour for n<1 is undefined but harmless) */

seq_t a(idx_t n)
{
    /* Offset 1: a(1)=1, a(2)=-8 */
    if (n == 0) {
        /* Not part of the defined offset but defined for safety */
        return (seq_t)0;
    }
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)-8;
    }

    /* Compute the sequence iteratively for n>=3 */
    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)-8;  /* a(2) */
    seq_t curr = 0;

    for (idx_t i = 3; i <= n; ++i) {
        curr = -(seq_t)8 * prev1 + (seq_t)9 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
