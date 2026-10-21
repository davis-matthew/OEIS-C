/*
 * A061998  Number of ways to place 5 nonattacking kings on an n X n board.
 * https://oeis.org/A061998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n < 5 a(n) = (n-4)*(n^9 + 4*n^8 - 74*n^7 - 176*n^6
 *            + 2411*n^5 + 1844*n^4 - 38194*n^3 + 18944*n^2 + 236520*n -
 *            316320) / 120 This matches OEIS A061998 for all n ≥ 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 15, u64 n <= 136, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Non‑attacking five kings fit only on boards of size ≥ 5. */
    if (n < 5) return 0;

    /* Compute the polynomial in 128‑bit arithmetic to avoid overflow. */
    unsigned __int128 n128 = n;
    unsigned __int128 n2   = n128 * n128;
    unsigned __int128 n3   = n2   * n128;
    unsigned __int128 n4   = n3   * n128;
    unsigned __int128 n5   = n4   * n128;
    unsigned __int128 n6   = n5   * n128;
    unsigned __int128 n7   = n6   * n128;
    unsigned __int128 n8   = n7   * n128;
    unsigned __int128 n9   = n8   * n128;

    /* Assemble the polynomial.  Casting to signed __int128 keeps the
       negative terms correctly represented. */
    __int128 poly =
        (__int128)n9
      + (__int128)4   * n8
      - (__int128)74  * n7
      - (__int128)176 * n6
      + (__int128)2411 * n5
      + (__int128)1844 * n4
      - (__int128)38194 * n3
      + (__int128)18944 * n2
      + (__int128)236520 * n128
      - (__int128)316320;

    /* (n-4) is non‑negative for n≥5, so the product is non‑negative. */
    __int128 result = (__int128)(n - 4) * poly / 120;

    return (seq_t)result;
}
