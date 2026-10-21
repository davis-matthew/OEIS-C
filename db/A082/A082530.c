/*
 * A082530  a(1)=1, a(n)=5*a(n-1)+1 if a(n-1) is odd, a(n)=a(n-1)/2+1
 *          otherwise.
 * https://oeis.org/A082530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>=2, a(n)= (a(n-1) is odd ? 5*a(n-1)+1 : a(n-1)/2+1) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined, but harmless */

    seq_t cur = 1;                    /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        if (cur & 1)                  /* cur is odd */
            cur = cur * 5 + 1;
        else                          /* cur is even */
            cur = cur / 2 + 1;
    }

    return cur;
}
