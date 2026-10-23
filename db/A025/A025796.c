/*
 * A025796  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^6)).
 * https://oeis.org/A025796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n ≥ 0, set k = n / 6 and r = n % 6. If r == 1 → a(n) =
 *            k·(k+1) / 2 Otherwise → a(n) = (k+1)·(k+2) / 2 (All calculations
 *            are performed in seq_t.)
 * Range:     u8 n <= 131, u16 n <= 2165, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count partitions of n into parts 2,3,6. */
seq_t a(idx_t n)
{
    idx_t k = n / 6u;          /* quotient (non‑negative) */
    uint8_t r = (uint8_t)(n % 6u);  /* remainder 0..5 */

    if (r == 1u) {
        /* k*(k+1)/2, using seq_t arithmetic */
        return (seq_t)k * ((seq_t)k + 1) / 2u;
    } else {
        /* (k+1)*(k+2)/2, using seq_t arithmetic */
        return ((seq_t)(k + 1u)) * ((seq_t)(k + 2u)) / 2u;
    }
}
