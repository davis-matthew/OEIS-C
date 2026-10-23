/*
 * A189640  Fixed point of the morphism 0->001, 1->101.
 * https://oeis.org/A189640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k-2) = a(k) for k ≥ 1 * a(3k-1) = 0 * a(3k) = 1 * with a(1) =
 *            0. * This implements the recurrence directly, reducing n by a
 *            factor of ~3 each step
 * Range:     u8 n <= 122, u16 n <= 122, u32 n <= 122, u64 n <= 122,
 *            u128 n <= 122, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial base case safely – the sequence is defined for n≥1. */
    if (n <= 1ULL)   /* includes n==0, just in case */
        return (seq_t)0;

    while (1) {
        /* n % 3 == 2  → a(n) = 0 */
        if (n % 3ULL == 2ULL)
            return (seq_t)0;

        /* n % 3 == 0  → a(n) = 1 */
        if (n % 3ULL == 0ULL)
            return (seq_t)1;

        /* n % 3 == 1  → a(n) = a((n+2)/3) ;  n > 1 guarantees progress */
        n = (n + 2ULL) / 3ULL;
        /* when n becomes 1, the loop will terminate via the first test above */
    }
}
