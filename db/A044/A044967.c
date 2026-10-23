/*
 * A044967  Numbers whose base-3 representation contains no 0's and exactly
 *          two 1's.
 * https://oeis.org/A044967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth smallest integer whose base‑3 representation *
 *            contains no 0's and exactly two 1's. The values are generated *
 *            by determining the length L of the base‑3 number and the two *
 *            positions of the digit 1. For a given length L the numbers * are
 *            sorted by having the 1's as high as possible, w...
 * Range:     u8 n <= 20, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0 is not part of the sequence (offset is 1) */
    if (n == 0) return 0;

    /* 1. Find the minimal length L such that C(L+1,3) >= n. */
    idx_t L = 2;                       /* at least two digits */
    while (1) {
        idx_t cum = (idx_t)(L + 1) * L * (L - 1) / 6; /* C(L+1,3) */
        if (cum >= n) break;
        ++L;
    }

    /* 2. Determine the rank k of the target within the block of
     * numbers having length L (1‑based). */
    idx_t prev = (idx_t)L * (L - 1) * (L - 2) / 6; /* C(L,3) */
    idx_t k = (idx_t)(n - prev);                  /* 1‑based */

    /* 3. Map that rank to a pair of positions (p,q), 0 <= p < q < L.
     * Enumeration order: q descending, then p descending. */
    idx_t k0 = k - 1;          /* 0‑based */
    idx_t p = 0, q = 0;
    for (idx_t qq = L - 1; qq > 0; --qq) {
        if (k0 < qq) {
            p = qq - 1 - k0;
            q = qq;
            break;
        }
        k0 -= qq;
    }

    /* 4. Compute the value:  val = 3^L - 1 - 3^p - 3^q. */
    seq_t powL = 1;
    for (idx_t i = 0; i < L; ++i) powL *= (seq_t)3;

    seq_t powP = 1;
    for (idx_t i = 0; i < p; ++i) powP *= (seq_t)3;

    seq_t powQ = 1;
    for (idx_t i = 0; i < q; ++i) powQ *= (seq_t)3;

    return powL - (seq_t)1 - powP - powQ;
}
