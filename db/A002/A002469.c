/*
 * A002469  The game of Mousetrap with n cards: the number of permutations of
 *          n cards in which 2 is the only hit.
 * https://oeis.org/A002469
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (n-3)*A000166(n-2) + (n-4)*A000166(n-3), with A000166(k)
 *            = (k-1)*(A000166(k-1) + A000166(k-2)), A000166(0) = 1,
 *            A000166(1) = 0
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    seq_t prev = 1; /* D(k-1) */
    seq_t cur = 0;  /* D(k), starting at k = 1 */
    idx_t k;
    for (k = 2; k <= n - 2; k++) {
        seq_t next = (seq_t)(k - 1) * (cur + prev);
        prev = cur;
        cur = next;
    }
    /* cur = D(n-2), prev = D(n-3) */
    return (seq_t)(n - 3) * cur + (seq_t)(n - 4) * prev;
}
