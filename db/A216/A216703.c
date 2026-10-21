/*
 * A216703  a(n) = Product_{k=1..n} (49 - 7/k).
 * https://oeis.org/A216703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} (49 - 7/k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t factor = 49 * k - 7;          /* 49*k-7 fits in idx_t for n ≤ 10000 */

#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
        /* Use 128‑bit intermediate to avoid overflow on integer builds */
        unsigned __int128 prod = (unsigned __int128)res * factor;
        res = (seq_t)(prod / k);
#else
        /* Integer build without 128 support or arbitrary‑precision build */
        res = res * factor / k;
#endif
    }
    return res;
}
