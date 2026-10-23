/*
 * A018227  Magic numbers: atoms with full shells containing any of these
 *          numbers of electrons are considered electronically stable.
 * https://oeis.org/A018227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 12*n^2 + 25*n - 6 + (-1)^n*(3*n + 6))/12
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t N = (seq_t)n;
    seq_t parity = n & 1;          /* 1 if odd, 0 if even */
    seq_t s = 3 * N + 6;           /* 3*n + 6 */
    seq_t base = 2 * N * N * N     /* 2*n^3 */
               + 12 * N * N        /* 12*n^2 */
               + 25 * N            /* 25*n */
               - 6;               /* -6 */
    seq_t num = parity ? (base - s) : (base + s);
    return num / 12;
}
