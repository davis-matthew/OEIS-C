/*
 * A195524  Number of lower triangles of a 6X6 -n..n array with all row and
 *          column sums zero.
 * https://oeis.org/A195524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (14778698 n^10 + 73893490 n^9 + 175612710 n^8 * +
 *            259089900 n^7 + 262706766 n^6 + 191658306 n^5 * + 102471610 n^4
 *            + 39997280 n^3 + 11067336 n^2 * + 1998144 n + 181440) / 181440 *
 *            for n ≥ 1 (the sequence is undefined below the offset)
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for intermediate values to avoid overflow
       when seq_t is uint64_t; the final result is always an integer.
       Casting from the 128‑bit type to seq_t is safe because the value
       fits into the target type (or the build’s arbitrary‑precision type). */

    const unsigned long long coeffs[11] = {
        14778698ULL,   /* n^10 coefficient */
        73893490ULL,   /* n^9 */
        175612710ULL,  /* n^8 */
        259089900ULL,  /* n^7 */
        262706766ULL,  /* n^6 */
        191658306ULL,  /* n^5 */
        102471610ULL,  /* n^4 */
        39997280ULL,   /* n^3 */
        11067336ULL,   /* n^2 */
        1998144ULL,    /* n^1 */
        181440ULL      /* constant term */
    };
    const unsigned long long denom = 181440ULL;

    unsigned __int128 val = coeffs[0];
    unsigned __int128 nn  = (unsigned __int128)n;

    for (int i = 1; i < 11; ++i)
        val = val * nn + coeffs[i];

    val /= denom;
    return (seq_t)val;
}
