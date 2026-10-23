/*
 * A189632  Fixed point starting with 0 of the morphism 0->001, 1->100.
 * https://oeis.org/A189632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(3k-2)=a(k), a(3k-1)=0, a(3k)=1-a(k) for k>=1, a(1)=0
 * Range:     u8 n <= 122, u16 n <= 122, u32 n <= 122, u64 n <= 122,
 *            u128 n <= 122, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) recursively without dynamic allocation. */

seq_t a(idx_t n)
{
    /* base case */
    if (n == 1)
        return 0;

    /* n = 3k-1 -> a(n) = 0 */
    if (n % 3 == 2)
        return 0;

    /* n = 3k   -> a(n) = 1 - a(k)  */
    if (n % 3 == 0) {
        idx_t k = n / 3;
        return 1 - a(k);
    }

    /* n = 3k-2 -> a(n) = a(k)     */
    idx_t k = (n + 2) / 3;
    return a(k);
}
