/*
 * A225381  Elimination order of the first person in a Josephus problem.
 * https://oeis.org/A225381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)/2 for odd n; a(n) = a(n/2) + n/2 for even n
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursively compute a(n).  The depth of the recursion is O(log n),
 * which is negligible for the required limits. */
static seq_t a_recursive(idx_t n)
{
    if (n & 1ULL) /* odd */
        return (seq_t)(n + 1) / 2;
    else         /* even */
        return a_recursive(n >> 1) + (seq_t)(n >> 1);
}

seq_t a(idx_t n)
{
    return a_recursive(n);
}
