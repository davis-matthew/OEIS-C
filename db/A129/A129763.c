/*
 * A129763  a(n) = Sum_{k=1..n} binomial(n+k-1, n)^2 / n.
 * https://oeis.org/A129763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{k=1..n} binomial(n+k-1, n)^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 5, a(3) = 39, ... (OEIS A129763) */

seq_t a(idx_t n)
{
    /* guard against the off‑by‑one at the offset;  n ≥ 1 for real use */
    if (n == 0)
        return 0;

    /* Use 128‑bit arithmetic to avoid intermediate overflows.  */
    /* Even for maximum n where a(n) still fits 64 bits (n ≤ 18),
     * binomial values stay well below 2^64, so the 128‑bit type is safe. */
    unsigned __int128 sum = 0;
    unsigned __int128 c   = 1;            /* C(n+0, n) = 1 for k = 1 */

    for (idx_t k = 1; k <= n; ++k) {
        sum += c * c;                    /* add the square of the current binomial */

        if (k < n) {                    /* prepare C(n+k, n) for the next iteration */
            /* c ← c * (n + k) / k   (exact integer division)   */
            c = (c * (unsigned __int128)(n + k)) / (unsigned __int128)k;
        }
    }

    /* Divide the exact sum by n to obtain the required term.  */
    return (seq_t)(sum / (unsigned __int128)n);
}
