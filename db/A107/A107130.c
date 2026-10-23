/*
 * A107130  Replace each odd digit d of n with (d-1)/2.
 * https://oeis.org/A107130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = interpret the decimal digits of n after replacing each
 *            odd * digit d with (d-1)/2 (i.e., 1→0, 3→1, 5→2, 7→3, 9→4). This
 *            is the * definition of OEIS A107130. The function computes the
 *            value in O(#digits)
 * Range:     u8 n <= 259, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

extern seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t factor  = 1;
    idx_t x       = n;
    while (x > 0) {
        unsigned digit = x % 10;          /* extract least‑significant decimal digit */
        if (digit & 1)                    /* odd?  replace with (d-1)/2 */
            digit = (digit - 1) >> 1;     /* equivalent to (digit-1)/2 */
        result += (seq_t)digit * factor;  /* accumulate into result */
        factor *= 10;
        x /= 10;
    }
    return result;
}
