/*
 * A053396  The full list of 5-Kaprekar numbers.
 * https://oeis.org/A053396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) is the nth integer whose square can be split into a left
 *            part * and a right part of 5 decimal digits so that the two
 *            parts sum to the * original number. Equivalently, n(n‑1) must be
 *            divisible by 99999
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t cache[8];
    static idx_t count = 0;
    if (count == 0) {
        /* precompute all 5‑Kaprekar numbers (there are exactly 8 of them) */
        for (idx_t i = 1; i <= 100000; ++i) {
            if ((i * (i - 1)) % 99999 == 0) {
                cache[count++] = (seq_t)i;
                if (count == 8) break;
            }
        }
    }
    if (n > 0 && n <= count)
        return cache[n - 1];
    return 0;
}
