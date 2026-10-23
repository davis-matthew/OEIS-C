/*
 * A045798  Oddish numbers (prime to 10 and 10's digit is odd).
 * https://oeis.org/A045798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = 20*floor((n-1)/4) + {11,13,17,19}[(n-1)%4]
 * Range:     u8 n <= 50, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If called with n == 0, return 0 – behaviour below offset is irrelevant. */
    if (n == 0) return 0;

    /* Compute quotient and remainder for (n-1)/4 */
    idx_t k = (n - 1) / 4;          /* block index */
    unsigned r = (unsigned)((n - 1) % 4);  /* position within block */

    /* Offsets for the four numbers in each 20‑number block. */
    static const seq_t offs[4] = {
        (seq_t)11,
        (seq_t)13,
        (seq_t)17,
        (seq_t)19
    };

    /* Base value for the block (multiple of 20). */
    seq_t base = (seq_t)20 * (seq_t)k;

    return base + offs[r];
}
