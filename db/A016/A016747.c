/*
 * A016747  a(n) = (2*n)^7.
 * https://oeis.org/A016747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 128 * n^7
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 11, u64 n <= 282,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute n^7 using binary exponentiation with a temporary 128‑bit
       variable to avoid intermediate overflow when n is small enough
       that the result still fits in seq_t.  If the final result does
       not fit the cast truncates as required. */
    seq_t pow_n7;
    {
        idx_t exp = 7;
        seq_t result = 1;
        seq_t base   = (seq_t)n;
        while (exp) {
            if (exp & 1ULL)
                result = (seq_t)((__int128)result * (__int128)base);
            base = (seq_t)((__int128)base * (__int128)base);
            exp >>= 1;
        }
        pow_n7 = result;
    }

    /* Multiply by 128, again using 128‑bit arithmetic to keep the
       intermediate exact for small values. */
    return (seq_t)((__int128)128 * (__int128)pow_n7);
}
