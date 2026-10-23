/*
 * A047214  Largest 6th power modulo n.
 * https://oeis.org/A047214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 232, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): largest residue of x^6 modulo n, by brute‑force
   enumeration of x = 0 .. n−1.  This satisfies the definition of
   OEIS sequence A047214. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* a(1) = 0 */

    seq_t mod = (seq_t)n;
    seq_t max = 0;

    for (idx_t ix = 0; ix < n; ++ix) {
        seq_t x  = (seq_t)ix;
        seq_t x2 = (x * x) % mod;        /* x^2 mod n */
        seq_t x4 = (x2 * x2) % mod;      /* x^4 mod n */
        seq_t x6 = (x4 * x2) % mod;      /* x^6 mod n */
        if (x6 > max)
            max = x6;
    }

    return max;
}
