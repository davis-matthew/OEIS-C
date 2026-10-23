/*
 * A037901  a(n) = (greatest base 7 digit of n) - (least base 7 digit of n).
 * https://oeis.org/A037901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (greatest base 7 digit of n) - (least base 7 digit of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* 0 has only digit 0 */

    idx_t max_d = 0;
    idx_t min_d = 7;                /* larger than any base‑7 digit */

    idx_t x = n;
    while (x)
    {
        idx_t d = x % 7;            /* current least significant base‑7 digit */
        if (d > max_d) max_d = d;
        if (d < min_d) min_d = d;
        x /= 7;
    }

    return (seq_t)(max_d - min_d);
}
