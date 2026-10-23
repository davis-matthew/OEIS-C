/*
 * A176895  Period 4: repeat [1, 4, 2, 4].
 * https://oeis.org/A176895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 / gcd(n,4) (period 4 sequence [1,4,2,4])
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n mod 4 determines the term */
    idx_t m = n & 3ULL;  /* same as n % 4 for unsigned integers */

    switch (m) {
        case 0: return (seq_t)1; /* a(0), a(4), a(8), ... */
        case 1: return (seq_t)4; /* a(1), a(5), a(9), ... */
        case 2: return (seq_t)2; /* a(2), a(6), a(10), ... */
        case 3: return (seq_t)4; /* a(3), a(7), a(11), ... */
        default: return (seq_t)0; /* never reached */
    }
}
