/*
 * A165989  Numbers such that n^2 = 29 mod 1193.
 * https://oeis.org/A165989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (-1193 -943*(-1)^n +2386*n)/4
 * Range:     u8 none, u16 n <= 110, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based */
    seq_t base, cnt;
    if (n & 1) {                     /* odd index */
        base = (seq_t)534;
        cnt  = (seq_t)((n - 1) / 2);
    } else {                         /* even index */
        base = (seq_t)659;
        cnt  = (seq_t)((n - 2) / 2);
    }

    /* compute cnt * 1193 without 64‑bit overflow */
    unsigned __int128 prod = (unsigned __int128)cnt * 1193;
    unsigned __int128 val  = (unsigned __int128)base + prod;

    /* final value fits in seq_t for tested ranges */
    return (seq_t)(unsigned long long)val;
}
