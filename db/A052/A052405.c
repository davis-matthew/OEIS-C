/*
 * A052405  Numbers without 3 as a digit.
 * https://oeis.org/A052405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = replace digits d>2 by d+1 in base‑9 representation of n-1
 * Range:     u8 n <= 203, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(1) = 0, a(2) = 1, a(3) = 2, a(4) = 4, a(13) = 14, etc. */

seq_t a(idx_t n)
{
    idx_t m = n - 1;        /* convert to 0‑based index */
    seq_t result = 0;
    seq_t factor = 1;       /* current 10^position */

    while (m > 0)
    {
        idx_t digit = m % 9;             /* base‑9 digit (0…8) */
        idx_t mapped = digit;
        if (digit >= 3)                   /* 3→4, 4→5, …, 8→9 */
            mapped = digit + 1;

        result += (seq_t)mapped * factor;
        factor *= 10;

        m /= 9;                          /* move to next base‑9 digit */
    }

    return result;
}
