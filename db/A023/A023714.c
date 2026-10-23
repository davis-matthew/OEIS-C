/*
 * A023714  Numbers with a single 2 in their base 4 expansion.
 * https://oeis.org/A023714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th positive integer whose base‑4 representation
 *            contains exactly one digit 2
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation scans successive integers and keeps those with exactly one digit 2. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, return 0 as a placeholder */

    seq_t candidate = 2;           /* first number with a single 2 is 2 (base4 "2") */
    idx_t found = 0;               /* count of valid numbers seen so far */

    for (;;)
    {
        seq_t tmp = candidate;
        idx_t two_cnt = 0;

        /* examine base‑4 digits of tmp */
        while (tmp != 0)
        {
            if (tmp % 4 == 2)   /* count digit 2 */
                ++two_cnt;

            tmp /= 4;
        }

        if (two_cnt == 1)        /* valid number */
        {
            ++found;
            if (found == n)      /* n-th such number reached */
                return candidate;
        }

        ++candidate;             /* test next integer */
    }
}
