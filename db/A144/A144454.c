/*
 * A144454  First trisection of A061039.
 * https://oeis.org/A144454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = (n²-1)/9 if n ≡ 1 or 8 (mod 9), * a(n) = (n²-1)/3 if n ≡
 *            1 or 2 (mod 3), * a(n) = n²-1 if n ≡ 0 (mod 3)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n² - 1 in the result type.  For the shifts in
     * the 64‑bit build the intermediate also fits in seq_t
     * because n is expected to be small (≤ 10 000 in the tests). */
    seq_t nn = (seq_t)n;
    seq_t num = nn * nn - (seq_t)1;

    /* Determine the correct divisor based on the residue of n */
    idx_t mod9 = n % 9ULL;
    if (mod9 == 1 || mod9 == 8) {
        return num / (seq_t)9;
    }

    idx_t mod3 = n % 3ULL;
    if (mod3 == 1 || mod3 == 2) {
        return num / (seq_t)3;
    }

    /* mod3 == 0 */
    return num;
}
