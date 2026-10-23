/*
 * A039038  Numbers whose base-7 representation has the same number of 1's and
 *          5's.
 * https://oeis.org/A039038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 122, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base‑7 representation has equal number of 1's and 5's
   a(n) = nth nonnegative integer x such that the number of digit 1 equals
   the number of digit 5 in x’s base‑7 representation. */
seq_t a(idx_t n)
{
    idx_t count = 0;        /* how many qualifying numbers have been seen */
    seq_t candidate = 0;    /* the current number to test */

    for (;; ++candidate)   /* iterate over non‑negative integers */
    {
        seq_t temp = candidate;
        unsigned c1 = 0, c5 = 0;

        /* Count digits 1 and 5 in the base‑7 representation of temp. */
        while (temp != 0) {
            unsigned d = (unsigned)(temp % 7);   /* current base‑7 digit */
            if (d == 1)  ++c1;
            else if (d == 5) ++c5;
            temp = temp / 7;
        }

        /* 0 has zero 1's and zero 5's, so it matches. */
        if (c1 == c5) {
            ++count;
            if (count == n)
                return candidate;
        }
    }
}
