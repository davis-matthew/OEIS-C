/*
 * A157078  a(n) = 32805000*n^2 - 55096200*n + 23133601.
 * https://oeis.org/A157078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32805000*n^2 - 55096200*n + 23133601
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute with 128‑bit intermediate to avoid overflow */
    unsigned __int128 t1 = (unsigned __int128)n * n;
    t1 *= 32805000ULL;
    unsigned __int128 t2 = (unsigned __int128)n * 55096200ULL;
    unsigned __int128 res = t1 + 23133601ULL - t2;
    return (seq_t)res;
}
