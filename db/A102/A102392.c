/*
 * A102392  Odious numbers in odious places.
 * https://oeis.org/A102392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = n when the binary representation of n has an odd number *
 *            of 1's (i.e., n is an odious number); otherwise a(n) = 0
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the “odious numbers in odious places” sequence A102392. */
seq_t a(idx_t n)
{
    /* Compute the parity (1 = odd popcount, 0 = even popcount). */
    unsigned parity = 0;
#if defined(__GNUC__) || defined(__clang__)
    /* GCC/Clang built‑in gives the popcount of an unsigned long long. */
    parity = (__builtin_popcountll((unsigned long long)n) & 1U);
#else
    /* Portable parity calculation: flip parity for each 1‑bit. */
    unsigned long long x = (unsigned long long)n;
    while (x) {
        parity ^= 1U;          /* toggle parity */
        x &= x - 1U;           /* drop lowest set bit */
    }
#endif

    /* Return n if it is odious, otherwise 0. */
    return parity ? (seq_t)n : (seq_t)0;
}
