/*
 * A019464  Multiply by 1, add 1, multiply by 2, add 2, etc., start with 1.
 * https://oeis.org/A019464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 40, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A019464: Multiply by 1, add 1, multiply by 2, add 2, …
 * Recurrence:
 *   a(0) = 1
 *   for n ≥ 1:
 *     if n is odd:  a(n) = a(n-1) * ((n+1)/2)
 *     if n is even: a(n) = a(n-1) + (n/2)
 */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        if ((i & 1) == 1) {        /* odd n → multiply */
            seq_t mul = (seq_t)((i + 1) / 2);
            cur = cur * mul;
        } else {                   /* even n → add */
            seq_t add = (seq_t)(i / 2);
            cur = cur + add;
        }
    }
    return cur;
}
