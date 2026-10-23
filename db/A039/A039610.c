/*
 * A039610  Numbers whose base-12 representation has the same nonzero number
 *          of 1's and 8's.
 * https://oeis.org/A039610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-12 representation has the same non‑zero
   number of 1's and 8's.
   OEIS A039610.  a(1) = 20, a(2) = 97, … */

/* Implementation: iterate through natural numbers, counting
   occurrences of digit 1 and digit 8 in the base‑12 expansion.
   Return the n‑th such number.  This is efficient enough for
   n up to at least 10 000 and works for all supported seq_t types. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    for (seq_t x = 1; ; ++x)
    {
        idx_t c1 = 0, c8 = 0;
        seq_t y = x;
        while (y)
        {
            seq_t d = y % (seq_t)12;
            if (d == (seq_t)1) { ++c1; }
            else if (d == (seq_t)8) { ++c8; }
            y /= (seq_t)12;
        }
        if (c1 == c8 && c1 > 0)
        {
            ++found;
            if (found == n) return x;
        }
    }
}
