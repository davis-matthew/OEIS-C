/*
 * A184615  Positive parts of the nonadjacent forms for n.
 * https://oeis.org/A184615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 170, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:   a(n) = the sum of the positive 1‑digits in the
 *               non‑adjacent form (NAF) representation of n.
 *               This implementation follows the standard NAF
 *               construction: at each step if n is odd choose the
 *               digit (±1) such that the next value of n becomes
 *               even, otherwise the digit is 0.  The accumulated
 *               sum of the 2^i corresponding to digit = +1 forms
 *               the result a(n).  No separate tables or hard‑coded
 *               terms are used.  The algorithm runs in O(log n)
 *               time. */

seq_t a(idx_t n)
{
    seq_t pos = 0;       /* accumulated positive part            */
    seq_t pow = 1;       /* 2^i for the current bit position     */

    while (n != 0) {
        if ((n & 1ULL) != 0) {            /* n is odd */
            /* Choose digit: +1 if n mod 4 == 1 or n == 1, else -1 */
            if ((n & 3ULL) == 1) {        /* digit = +1 */
                pos += pow;              /* add 2^i to the positive part */
                n = (n - 1) >> 1;         /* n = (n - 1) / 2 */
            } else {                     /* digit = -1 */
                n = (n + 1) >> 1;         /* n = (n + 1) / 2, no addition to pos */
            }
        } else {                         /* n is even, digit = 0 */
            n >>= 1;                     /* n = n / 2 */
        }
        /* Move to the next bit position: pow = pow * 2 */
        pow = pow + pow;                  /* safe for unsigned seq_t */
    }

    return pos;
}
