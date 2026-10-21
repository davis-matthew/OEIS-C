/*
 * A076452  a(n+2) = abs(a(n+1)) - a(n), a(0)=0, a(1)=1.
 * https://oeis.org/A076452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(n+2)=abs(a(n+1))-a(n).  
   The sequence is periodic with period 9: a(n)=a(n-9). */

seq_t a(idx_t n)
{
    static seq_t period[9];
    static int initialized = 0;

    if (!initialized) {
        period[0] = 0;
        period[1] = 1;
        for (idx_t i = 2; i < 9; ++i) {
            seq_t prev1 = period[i-1];
            seq_t prev2 = period[i-2];
            seq_t absprev1 = (prev1 >= 0 ? prev1 : -prev1);
            period[i] = absprev1 - prev2;
        }
        initialized = 1;
    }

    return period[n % 9];
}
