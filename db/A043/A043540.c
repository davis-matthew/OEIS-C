/*
 * A043540  Number of distinct base-13 digits of n.
 * https://oeis.org/A043540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct digits in base-13 representation of n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The base‑13 representation of n is formed by repeated
       division by 13.  A 13‑bit mask records which digits have
       appeared; the number of distinct digits is the popcount of
       that mask.  All arithmetic is done in idx_t or native
       unsigned types, preserving the exactness required. */
    idx_t tmp = n;
    unsigned mask = 0;         /* 13‑bit mask for digits 0…12 */

    while (tmp > 0) {
        unsigned d = (unsigned)(tmp % 13); /* digit in current position */
        mask |= 1u << d;
        tmp /= 13;
    }

    /* Count the set bits in the mask. */
    unsigned count = 0;
    while (mask) {
        count += mask & 1u;
        mask >>= 1;
    }

    return (seq_t)count;
}
