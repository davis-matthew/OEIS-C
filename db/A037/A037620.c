/*
 * A037620  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,3.
 * https://oeis.org/A037620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1)=2;  for n>1, a(n)=6*a(n-1)+d_n,
   where d_n = {2,0,3}[(n-1)%3]  (the periodic base‑6 digits 2,0,3). */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* valid for safety, offset is 1 */
    seq_t val = 0;
    static const unsigned char digits[3] = {2, 0, 3};

    for (idx_t i = 1; i <= n; ++i)
    {
        unsigned int digit = digits[(int)((i - 1) % 3)];
        val = val * 6 + digit;
    }
    return val;
}
