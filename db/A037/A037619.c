/*
 * A037619  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,3.
 * https://oeis.org/A037619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*a(n-1)+d[n-1] with a(1)=2 and d[k] = {2,0,3}[k mod 3]
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n) is the integer whose base‑5 representation consists of the first n
   digits of the periodic sequence 2,0,3,2,0,3,... */

seq_t a(idx_t n)
{
    seq_t res = 0;
    static const seq_t digits[3] = { 2, 0, 3 };

    /* Process full groups of three digits at a time: (2,0,3) */
    idx_t i;
    for (i = 0; i + 3 <= n; i += 3)
    {
        /* (res * 5 + 2) * 5 + 0
           Resulting in res * 125 + 53 for one full cycle (2,0,3) */
        res = res * 125 + 53;
    }

    /* Handle the remaining 0, 1 or 2 digits */
    idx_t rem = n - i;
    for (i = 0; i < rem; ++i)
    {
        res = res * 5 + digits[i];
    }

    return res;
}
