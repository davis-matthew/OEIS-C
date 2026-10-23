/*
 * A043534  Number of distinct base-7 digits of n.
 * https://oeis.org/A043534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct base‑7 digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1, but handle 0 safely */

    unsigned mask = 0;
    idx_t m = n;
    while (m) {
        unsigned d = (unsigned)(m % 7);
        mask |= 1u << d;
        m /= 7;
    }

    /* count set bits in mask (maximum 7 bits) */
    unsigned cnt = 0;
    while (mask) {
        cnt++;
        mask &= mask - 1;
    }
    return (seq_t)cnt;
}
