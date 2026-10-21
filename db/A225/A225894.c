/*
 * A225894  Number of n X 2 binary arrays whose sum with another n X 2 binary
 *          array containing no more than a single 1 has rows and columns in
 *          lexicographically nondecreasing order.
 * https://oeis.org/A225894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + n^3 + 8 n^2 + 2 n + 12)/6
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 4, a(2) = 12, a(3) = 33, ... */
seq_t a(idx_t n)
{
    /* For n >= 1 the returned value is exact. */
    if (n == 0) {
        /* Undefined for n=0, but return 0 to keep the function well‑defined. */
        return (seq_t)0;
    }

    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;                 /* n^2 */
    seq_t n3 = n2 * N;                /* n^3 */
    seq_t n4 = n2 * n2;               /* n^4 */

    seq_t num = n4 + n3 + (seq_t)8 * n2 + (seq_t)2 * N + (seq_t)12;
    return num / (seq_t)6;
}
