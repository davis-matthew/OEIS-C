/*
 * A032916  Numbers whose set of base-9 digits is {1,3}.
 * https://oeis.org/A032916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032916: numbers whose set of base‑9 digits is {1,3}
   (ordered increasingly). */

seq_t a(idx_t n)
{
    /* Find the length k of the base‑9 representation.
       The total number of terms up to length k is 2^(k+1)-2,
       so we need the smallest k with 2^(k+1) >= n+2. */
    idx_t p = 1;           /* 2^0   */
    idx_t m = 0;           /* m = log2(p) */
    idx_t target = n + (idx_t)2;
    while (p < target) {
        p <<= 1;
        ++m;
    }
    idx_t k = m - 1;        /* number of base‑9 digits */

    /* Offset of n among the k‑digit numbers (0‑based). */
    idx_t pref = p >> 1;    /* 2^k */
    idx_t offset0 = n - pref + 1;

    /* Convert the offset to a number whose base‑9 digits are
       1 (for bit 0) or 3 (for bit 1). */
    seq_t val = 0;
    for (idx_t i = 0; i < k; ++i) {
        idx_t bit = (offset0 >> (k - 1 - i)) & 1;
        val = val * 9 + (bit ? 3 : 1);
    }
    return val;
}
