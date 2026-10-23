/*
 * A032070  Number of reversible strings with n labeled beads of 3 colors, no
 *          palindromes of more than 1 bead.
 * https://oeis.org/A032070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * f(n) where f(1)=3, f(2)=3, f(3)=9, and for n≥4: if n
 *            is even: f(n) = 3*f(n-1) + 3^(n/2) if n is odd: f(n) = 3*f(n-1)
 *            Then a(n) = n! * f(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t factorial = 1;     /* n!          */
    seq_t f = 3;             /* f(n) value  */
    seq_t pow3 = 1;          /* 3^(i/2) for current even i */

    if (n == 1)              /* base case */
        return factorial * f;    /* 1! * 3 = 3 */

    for (idx_t i = 2; i <= n; ++i)
    {
        factorial *= (seq_t)i;

        /* update 3^(i/2) before using it for even i */
        if ((i & 1) == 0)            /* i even */
            pow3 *= (seq_t)3;

        if (i == 2)
            f = 3;
        else if (i == 3)
            f = 9;
        else
        {
            if ((i & 1) == 0)        /* even i */
                f = 3 * f + pow3;
            else                      /* odd i */
                f = 3 * f;
        }
    }

    return factorial * f;
}
