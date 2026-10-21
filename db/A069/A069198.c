/*
 * A069198  a(1)=1 a(2)=6 a(n+2)=(a(n+1)+a(n))/3 if (a(n+1)+a(n)==0 (mod 3));
 *          a(n+2)=(a(n+1)+a(n))/2 if (a(n+1)+a(n)==0 (mod 2));
 *          a(n+2)=a(n+1)+a(n) otherwise.
 * https://oeis.org/A069198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=6, a(n+2) = (a(n+1)+a(n))/3 if a(n+1)+a(n) ≡ 0 (mod
 *            3) (a(n+1)+a(n))/2 if a(n+1)+a(n) ≡ 0 (mod 2) a(n+1)+a(n)
 *            otherwise (offset 1)
 * Range:     u8 n <= 42, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;

    /* Use two variables to hold the last two values of the sequence. */
    seq_t a_prev = (seq_t)1;   /* a(n-2) */
    seq_t a_curr = (seq_t)6;   /* a(n-1) */
    seq_t a_next, sum;

    /* Iterate from the 3rd term up to the requested index. */
    for (idx_t i = 3; i <= n; ++i)
    {
        sum = a_prev + a_curr;

        if (sum % (seq_t)3 == 0)
            a_next = sum / (seq_t)3;
        else if (sum % (seq_t)2 == 0)
            a_next = sum / (seq_t)2;
        else
            a_next = sum;

        /* Slide the window: a(n-1) becomes a(n-2), a(n) becomes a(n-1). */
        a_prev = a_curr;
        a_curr = a_next;
    }

    /* After the loop, a_curr holds a(n). */
    return a_curr;
}
