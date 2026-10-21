/*
 * A053638  a(n) = ceiling(2^n/n).
 * https://oeis.org/A053638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(2^n / n)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 70, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – behaviour below 1 is irrelevant */

#ifdef __SIZEOF_INT128__
    /* use 128‑bit intermediate when possible to avoid overflow for
       small n. The shift is only valid for n < 128. */
    if (n < 128) {
        unsigned __int128 num = (unsigned __int128)1 << n;
        seq_t  res = (seq_t)(num / n);
        if ((num % n) != 0) res += 1;
        return res;
    }
#endif

    /* fallback: compute 2^n by repeated multiplication.
       Works for arbitrary‑precision seq_t; for small fixed‑width
       types (tests stop when the result does not fit) it stays
       within range. */
    seq_t num = 1;
    for (idx_t i = 0; i < n; ++i) num *= 2;
    seq_t res = num / n;
    if (num % n != 0) res += 1;
    return res;
}
