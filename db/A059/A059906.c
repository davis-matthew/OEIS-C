/*
 * A059906  Index of second half of decomposition of integers into pairs based
 *          on A000695.
 * https://oeis.org/A059906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, look, nonn
 * Formula:   a(n) = Σ_{j≥0} b_{2j+1}·2^j where n = Σ b_k·2^k (binary
 *            expansion). This is the compression of the odd‑indexed bits of n
 *            into a new binary number
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 the result is 0. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Find the most significant bit index of n (0‑based). */
    int msb = 63 - __builtin_clzll((unsigned long long)n);

    /* If the highest bit is at an even index, no odd bit contributes. */
    if (msb == 0)                 /* n has only bit 0 set (i.e., n == 1) */
        return (seq_t)0;

    /* Determine the highest odd position ≤ msb. */
    int max_pos = (msb & 1) ? msb : msb - 1;  /* 1 if msb odd, else msb‑1 */

    seq_t result = 0;

    /* Process odd positions from most significant down to 1.
       For each such position, take the bit and append it to result. */
    for (int pos = max_pos; pos >= 1; pos -= 2) {
        seq_t bit = (seq_t)((n >> pos) & 1ULL);
        result = (result << 1) | bit;
    }
    return result;
}
