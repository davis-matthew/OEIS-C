/*
 * A182582  a(n) = (A096268(n-1) + A182581(n)) mod 2.
 * https://oeis.org/A182582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000035(A169611(n))
 * Range:     u8 n <= 9275, u16 n <= 9275, u32 n <= 9275, u64 n <= 9275,
 *            u128 n <= 9275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A169611(n) = ν₂(n) + ν₃(n); a(n) is its parity. */

seq_t a(idx_t n)
{
    idx_t t = n;
    seq_t parity = 0;  /* 0 or 1 */

    /* Count parity of powers of 2. */
    while ((t & 1ULL) == 0)
    {
        parity ^= 1;
        t >>= 1;
    }

    /* Count parity of powers of 3. */
    while (t % 3ULL == 0)
    {
        parity ^= 1;
        t /= 3;
    }

    return parity;
}
