/*
 * A085111  a(1) = 1; then add 1, multiply by 2, subtract 3, multiply by 4,
 *          add 5, multiply by 6, subtract 7, multiply by 8 and so on.
 * https://oeis.org/A085111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 22, u64 n <= 34, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1;
   For n>1:
     if n is odd: a(n)=a(n-1)*(n-1);
     else if (n/2) is odd: a(n)=a(n-1)+(n-1);
     else:                  a(n)=a(n-1)-(n-1); */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, return 0 */
    seq_t val = 1;                 /* a(1) */
    if (n == 1) return val;

    for (idx_t i = 2; i <= n; ++i)
    {
        if ((i & 1) == 1)          /* odd i */
        {
            val *= (seq_t)(i - 1);
        }
        else                       /* even i */
        {
            idx_t k = i >> 1;      /* k = i/2 */
            if (k & 1)             /* k odd -> add */
            {
                val += (seq_t)(i - 1);
            }
            else                   /* k even -> subtract */
            {
                val -= (seq_t)(i - 1);
            }
        }
    }
    return val;
}
