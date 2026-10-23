/*
 * A045283  Numbers whose base-5 representation contains exactly one 2 and two
 *          4's.
 * https://oeis.org/A045283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): numbers whose base‑5 representation contains exactly one 2 and two 4's.
   Implemented by brute‑force search. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n < 1, return 0 as guard */

    uint64_t count = 0;          /* number of terms found so far */
    for (idx_t val = 1; ; ++val)
    {
        /* Inspect the base‑5 digits of val */
        seq_t t = (seq_t)val;
        unsigned cnt2 = 0, cnt4 = 0;

        while (t > 0)
        {
            seq_t d = t % 5;
            if (d == (seq_t)2) ++cnt2;
            else if (d == (seq_t)4) ++cnt4;
            t = t / 5;
        }

        if (cnt2 == 1 && cnt4 == 2)
        {
            ++count;
            if (count == n)
                return (seq_t)val;
        }
    }
}
