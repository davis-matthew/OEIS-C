/*
 * A128999  Start with an integer (in this case 1). First, add 5 or 6 if the
 *          integer is odd or even, respectively. Then divide by 2.
 * https://oeis.org/A128999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1; a(n) = (a(n-1)+(a(n-1)%2?5:6))/2 for n≥2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence stabilises at 5 after a(4). */
seq_t a(idx_t n)
{
    /* For n >= 4 the sequence has reached its fixed point 5. */
    if (n >= 4)
        return (seq_t)5;

    seq_t val = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
    {
        if ((val & (seq_t)1) != 0)   /* odd */
            val = (val + (seq_t)5) / (seq_t)2;
        else                         /* even */
            val = (val + (seq_t)6) / (seq_t)2;
    }
    return val;
}
