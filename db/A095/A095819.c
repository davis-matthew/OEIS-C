/*
 * A095819  Tenth column (m=9) of (1,4)-Pascal triangle A095666.
 * https://oeis.org/A095819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 36) * binomial(n + 8, 8) / 9
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Compute binomial(n+8, 8) multiplicatively using 128‑bit arithmetic
    unsigned __int128 bin = 1;
    for (idx_t i = 1; i <= 8; ++i) {
        unsigned __int128 term = (unsigned __int128)(n + 9 - i); // n+8, n+7, ..., n+1
        bin = bin * term / i;                                  // exact division
    }

    // Compute (n+36) * bin / 9, exact in 128‑bit
    unsigned __int128 result = bin * (unsigned __int128)(n + 36);
    result /= 9;

    return (seq_t)result;   // truncation only occurs when the value does not fit in seq_t
}
