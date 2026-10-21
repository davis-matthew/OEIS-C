/*
 * A079423  Marks on lexicographically earliest 8-mark optimal Golomb ruler.
 * https://oeis.org/A079423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 0,1,4,9,15,22,32,34 for n=1..8 (otherwise 0) – marks of
 *            the lexicographically earliest optimal 8‑mark Golomb ruler
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t vals[] = {0,1,4,9,15,22,32,34};
    if (n == 0) return 0;           /* out of range – keep behavior defined */
    /* No defined terms for n > 8, return 0 */
    if (n <= (sizeof(vals)/sizeof(vals[0]))) {
        return vals[n-1];
    }
    return 0;
}
