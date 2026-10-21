/*
 * A071413  a(n) = if n=0 then 0 else a(floor(n/2))+n*(-1)^(n mod 2).
 * https://oeis.org/A071413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 78, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n>0: a(n)=a(floor(n/2))+n*(-1)^(n mod 2) */
seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t cur = n;
    while (cur)
    {
        if ((cur & 1) == 0)          /* even */
            result += (seq_t)cur;
        else                         /* odd  */
            result -= (seq_t)cur;
        cur >>= 1;                   /* cur = floor(cur/2) */
    }
    return result;
}
