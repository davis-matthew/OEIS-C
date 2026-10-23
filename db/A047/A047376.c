/*
 * A047376  Numbers that are congruent to {1, 4, 5} mod 7.
 * https://oeis.org/A047376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(3k) = 7k-2 * a(3k-1)= 7k-3 * a(3k-2)= 7k-6 * where a(1)=1,
 *            a(2)=4, a(3)=5, etc
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n) only defined for n >= 1 */
    seq_t result;
    idx_t k;

    switch (n % 3u) {
    case 1u: /* n = 3k-2  => k = (n+2)/3 */
        k = (n + 2u) / 3u;
        result = (seq_t)7 * (seq_t)k - (seq_t)6;
        break;
    case 2u: /* n = 3k-1  => k = (n+1)/3 */
        k = (n + 1u) / 3u;
        result = (seq_t)7 * (seq_t)k - (seq_t)3;
        break;
    case 0u: /* n = 3k    => k = n/3   */
        k = n / 3u;
        result = (seq_t)7 * (seq_t)k - (seq_t)2;
        break;
    default: /* unreachable, but keep compiler happy */
        result = 0;
        break;
    }

    return result;
}
