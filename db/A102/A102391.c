/*
 * A102391  Evil numbers in evil places.
 * https://oeis.org/A102391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = if((1+floor(n/2))*(1+(-1)^A000120(n))/2 == 0, 0, n) This
 *            simplifies to: return n if n has an even number of 1‑bits (i.e.,
 *            n is an evil number), otherwise return 0
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#if defined(__GNUC__) || defined(__clang__)
    /* Use the compiler builtin for parity of a 64‑bit integer. */
    return (__builtin_parityll(n) == 0) ? (seq_t)n : 0;
#else
    /* Portable parity calculation. */
    idx_t tmp = n;
    int parity = 0;
    while (tmp) {
        parity ^= 1;          /* flip parity for each set bit */
        tmp &= tmp - 1;       /* remove lowest set bit */
    }
    return (parity == 0) ? (seq_t)n : 0;
#endif
}
