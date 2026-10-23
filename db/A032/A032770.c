/*
 * A032770  Integer quotients of n(n + 1)(n + 2)(n + 3)(n + 4) /
 *          (n+(n+1)+(n+2)+(n+3)+(n+4)).
 * https://oeis.org/A032770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let k = 5*(n/4)+{0,1,2,4}[n%4]; then a(n) = k(k+1)(k+3)(k+4)/5
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Map index n to the actual integer k that satisfies the expression. */
    static const int offset[4] = {0, 1, 2, 4};
    idx_t block = n / 4;          /* number of complete blocks of 4 terms */
    int pos    = n % 4;           /* position within the block (0‑3)   */
    seq_t k    = (seq_t)5 * (seq_t)block + (seq_t)offset[pos];

    /* Compute a(n) = k*(k+1)*(k+3)*(k+4)/5 */
    seq_t a = k * (k + 1);
    a      = a * (k + 3);
    a      = a * (k + 4);
    a      = a / 5; /* guaranteed integer division */
    return a;
}
