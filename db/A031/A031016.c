/*
 * A031016  Write n in base 7, then complement each digit (d -> 6-d) and
 *          juxtapose.
 * https://oeis.org/A031016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth digit of the infinite string obtained by writing
 *            1,2,3,… in base 7, complementing each digit (d → 6‑d), and
 *            concatenating
 * Range:     u8 n <= 843, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t cumulative = 0;
    for (idx_t k = 1; ; ++k) {
        /* Number of base‑7 digits of k */
        idx_t temp = k;
        idx_t f = 1;
        while (temp >= 7) {
            temp /= 7;
            ++f;
        }
        cumulative += f;
        if (cumulative >= n) {
            /* Position of the desired digit within k (0‑based from most significant) */
            idx_t e = cumulative - n;   /* 0 <= e < f */
            seq_t power = 1;
            for (idx_t i = 0; i < e; ++i) {
                power *= 7;
            }
            seq_t ak = (seq_t)k;
            seq_t digit = (ak / power) % 7;
            return (seq_t)(6 - digit);
        }
    }
}
