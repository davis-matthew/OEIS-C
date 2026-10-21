/*
 * A205593  a(2) = 0, a(3k) = a(3k+1) = a(2k), a(3k+2) = a(2k+1) + 1 for k >=
 *          1.
 * https://oeis.org/A205593
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2)=0; For k>=1: a(3k)=a(3k+1)=a(2k); a(3k+2)=a(2k+1)+1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* undefined behavior for n<2, but return 0 to be safe */
    if (n < 2) return 0;

    seq_t res = 0;
    while (n > 2) {
        idx_t k = n / 3;
        idx_t r = n % 3;
        if (r == 2) {
            res += 1;
            n = 2 * k + 1;
        } else { /* r == 0 or r == 1 */
            n = 2 * k;
        }
    }
    return res;
}
