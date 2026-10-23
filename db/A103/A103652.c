/*
 * A103652  Three-digit numbers with nonincreasing decimal digits such that
 *          the first digit modulo the second digit equals the third digit.
 * https://oeis.org/A103652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, nonn, full
 * Formula:   a(n) = the n‑th smallest 3‑digit integer abc with a ≥ b ≥ c and
 *            a mod b = c (ascending order)
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */
    idx_t count = 0;
    for (idx_t a = 1; a <= 9; ++a)        /* first digit: 1…9 */
    {
        for (idx_t b = 1; b <= a; ++b)    /* second digit: 1…a (b>0) */
        {
            idx_t r = a % b;              /* third digit = remainder */
            seq_t val = (seq_t)a * 100 + (seq_t)b * 10 + (seq_t)r;
            ++count;
            if (count == n) return val;
        }
    }
    return 0;   /* n beyond the finite length of the sequence */
}
