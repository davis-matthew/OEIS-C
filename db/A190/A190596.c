/*
 * A190596  Maximal digit in base-7 expansion of n.
 * https://oeis.org/A190596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximal digit in the base‑7 expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0, a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 4, a(5) = 5, a(6) = 6, ... */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    seq_t maxd = 0;          /* maximum digit found so far (0 to 6) */
    idx_t  x = n;            /* value to be examined */

    /* repeatedly take the least significant base‑7 digit */
    while (x > 0) {
        idx_t d = x % 7;     /* the current digit (0..6) */
        if (d > maxd) {
            maxd = d;
        }
        x /= 7;              /* shift right one digit in base 7 */
    }

    return maxd;
}
