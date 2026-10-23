/*
 * A140268  a(n) = negative integer -n presented in balanced ternary system.
 * https://oeis.org/A140268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the digits of -n written in balanced ternary, where the
 *            digit -1 is represented by the decimal digit 2, 0 by 0, and 1 by
 *            1
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. */
    if (n == 0)
        return 0;   /* undefined for n=0, but return 0 to avoid UB */

    /* Convert n to balanced ternary digits. */
    seq_t m = (seq_t)n;
    /* The number of base‑3 digits for any 64‑bit integer is at most 40.
       For arbitrarily large seq_t we still need a static limit – 64
       is more than enough for any realistic test. */
    int digits[64];
    int digits_cnt = 0;

    while (m > 0)
    {
        int r = (int)(m % 3);   /* remainder 0,1,2 */
        m /= 3;
        if (r == 2)              /* treat 2 as -1 with a carry */
        {
            r = -1;
            m += 1;
        }
        digits[digits_cnt++] = r;   /* r is now -1,0,1 */
    }

    /* Build the decimal number for -n by taking the digits in reverse
       order (most significant first) and flipping their signs. */
    seq_t result = 0;
    for (int i = digits_cnt - 1; i >= 0; --i)
    {
        int d  = digits[i];     /* balanced ternary digit of +n */
        int nd = -d;            /* balanced ternary digit of -n */
        int digit = (nd == -1) ? 2 : nd;  /* map to decimal digit 0,1,2 */
        result = result * 10 + digit;
    }

    return result;
}
