/*
 * A010344  Base-4 Armstrong or narcissistic numbers (written in base 10).
 * https://oeis.org/A010344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = nth base‑4 Armstrong number (written in base‑10). Let d
 *            be the number of digits of n in base 4; then n = Σ digit_i^d
 *            over all base‑4 digits of n
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t count = 0;
    idx_t cand = 1;
    while (1) {
        /* compute the number of base‑4 digits d */
        idx_t tmp = cand;
        idx_t d = 0;
        while (tmp > 0) {
            ++d;
            tmp /= 4;
        }
        /* compute sum of each digit raised to the d‑th power */
        tmp = cand;
        seq_t sum = 0;
        for (idx_t i = 0; i < d; ++i) {
            seq_t digit = tmp % 4;       /* digit is 0..3 */
            seq_t pow = 1;
            for (idx_t k = 0; k < d; ++k) pow *= digit;
            sum += pow;
            tmp /= 4;
        }
        if (sum == (seq_t)cand) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
        ++cand;
    }
}
