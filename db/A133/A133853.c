/*
 * A133853  a(n) = (64^n - 1)/63.
 * https://oeis.org/A133853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (64^n - 1)/63
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes a(n) = (64^n - 1)/63 = Σ_{j=0}^{n-1} 64^j
 * with seq_t being the sequence element type and idx_t the index type.
 * Works for all provided seq_t implementations (uint64, __int128,
 * arbitrary precision, etc.) and for any n ≥ 0.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t result = 0;
    seq_t power  = 1;          /* 64^0 */

    for (idx_t i = 0; i < n; ++i) {
        result += power;       /* add current power of 64 */
        power  *= (seq_t)64;   /* prepare next power (wraps for unsigned types) */
    }

    return result;
}
