/*
 * A061532  Nearest integer to n^7/49.
 * https://oeis.org/A061532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to n^7/49, i.e. a(n) = floor((n^7 +
 *            24)/49) for n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 41, u64 n <= 985, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset start at 1; for n==0 return 0 (undefined in the sequence) */
    if (n == 0) return 0;

    /* Use unsigned 128‑bit intermediate to avoid overflow for all n up to ~10000 */
    unsigned __int128 v = 1;
    unsigned __int128 nn = n;
    for (int i = 0; i < 7; ++i)
        v *= nn;               /* v = n^7 */

    unsigned __int128 r = (v + 24) / 49;   /* round to nearest integer */
    return (seq_t)r;
}
