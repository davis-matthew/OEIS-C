/*
 * A037787  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,0,1.
 * https://oeis.org/A037787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037787: Base 5 digits are, in order, the first n terms of the periodic
   sequence with initial period 3,2,0,1.  
   a(1) = 3, a(2) = 17, a(3) = 85, a(4) = 426, ...  
   Recurrence:
     a(n) = 5 * a(n-1) + d_n,
   where d_n is the nth digit of the repeating pattern [3, 2, 0, 1]. */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. */
    if (n == 0)
        return 0;   /* Undefined for n <= 0, but return 0 to satisfy the type. */

    /* The repeating digit pattern (base‑5 digits). */
    static const int pattern[4] = {3, 2, 0, 1};

    seq_t prev = 3;      /* a(1) */
    if (n == 1)
        return prev;

    for (idx_t i = 2; i <= n; ++i)
    {
        int digit = pattern[(i - 1) % 4];      /* nth digit (1‑based). */
        /* Compute a(i) = 5 * a(i-1) + digit. */
        prev = prev * (seq_t)5 + (seq_t)digit;
    }

    return prev;
}
