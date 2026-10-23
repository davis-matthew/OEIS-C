/*
 * A023703  Numbers with exactly 5 2's in their ternary expansion.
 * https://oeis.org/A023703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   sequence of numbers that contain exactly five 2’s in their
 *            ternary representation
 * Range:     u8 n <= 1, u16 n <= 8483, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pseudo‑formula: a(n) is the n‑th integer x ≥ 0 such that
       the base‑3 expansion of x contains exactly five digits equal
       to 2. */
    if (n == 0)
        return 0;          /* offset is 1, so n ≥ 1 in normal use */
    seq_t val = 0;         /* current candidate */
    idx_t found = 0;       /* how many suitable numbers we have seen */

    for (;; ++val)
    {
        seq_t tmp = val;
        idx_t twos = 0;
        while (tmp)
        {
            if (tmp % 3 == 2)
                ++twos;
            tmp /= 3;
        }
        if (twos == 5)
        {
            if (++found == n)
                return val;
        }
    }
    /* unreachable */
    return 0;
}
