/*
 * A049046  Number of k >= 1 with k! == 1 (mod n).
 * https://oeis.org/A049046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of integers k ≥ 1 with k! ≡ 1 (mod n)
 * Range:     u8 n <= 2730, u16 n <= 12021, u32 n <= 12021, u64 n <= 12021,
 *            u128 n <= 12021, big any n
 * Validated: standard, pass, 121 terms through n = 9550 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;
    seq_t count = 1;          /* k = 1 always gives 1 mod n for n > 1 */
    seq_t fac   = 1;
    seq_t mod   = (seq_t)n;

    for (idx_t i = 2; i < n; ++i) {
        fac = (fac * (seq_t)i) % mod;
        if (fac == 1)
            ++count;
    }
    return count;
}
