/*
 * A080782  a(1)=1, a(n)=a(n-1)-1 if n is already in the sequence,
 *          a(n)=a(n-1)+2 otherwise.
 * https://oeis.org/A080782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 3, a(3) = 2 Recurrence: a(n) = a(n-3) + 3 (n >
 *            3) Closed form derived from the recurrence: if (n mod 3 == 1)
 *            a(n) = n if (n mod 3 == 2) a(n) = n + 1 if (n mod 3 == 0) a(n) =
 *            n - 1
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)    /* out of bounds; behaviour undefined */
        return 0;

    idx_t r = n % 3;
    seq_t val;

    if (r == 1) {
        val = (seq_t)n;          /* a(n) = n          */
    } else if (r == 2) {
        val = (seq_t)n + 1;      /* a(n) = n + 1      */
    } else {                    /* r == 0,  n >= 3   */
        val = (seq_t)n - 1;      /* a(n) = n - 1      */
    }

    return val;
}
