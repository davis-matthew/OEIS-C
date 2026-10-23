/*
 * A199213  a(n) = (3*5^n+1)/2.
 * https://oeis.org/A199213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*5^n+1)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For small seq_t types (≤64 bits) use 128‑bit intermediates
       to avoid overflow while computing 5^n.  For larger types
       or arbitrary‑precision builds use seq_t arithmetic directly. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pow = 1;
        for (idx_t i = 0; i < n; ++i)
            pow *= 5;
        unsigned __int128 val = pow * 3 + 1;
        val /= 2;
        return (seq_t)val;
    } else {
        seq_t pow = 1;
        for (idx_t i = 0; i < n; ++i)
            pow *= 5;
        seq_t val = pow * 3 + 1;
        val /= 2;
        return val;
    }
}
