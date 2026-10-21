/*
 * A070232  a(1) = 4; a(n) = smallest composite number greater than the sum of
 *          all previous terms.
 * https://oeis.org/A070232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=6, a(3)=12, a(4)=24, a(5)=48; a(n)=95*2^(n-6) for
 *            n>=6
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against indices below the offset (not used in tests) */
    if (n == 0) return 0;

    /* First five terms are hard‑coded. */
    static const seq_t first[5] = {
        (seq_t)4,
        (seq_t)6,
        (seq_t)12,
        (seq_t)24,
        (seq_t)48
    };

    if (n <= 5)
        return first[n-1];

    /* For n >= 6 use the explicit formula. */
    seq_t value = (seq_t)95;
    idx_t shift = n - 6;          /* number of times to double */

    for (idx_t i = 0; i < shift; ++i)
        value = value * (seq_t)2; /* multiply by 2 each step */

    return value;
}
