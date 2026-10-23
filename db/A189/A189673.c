/*
 * A189673  Fixed point of the morphism 0->010, 1->110.
 * https://oeis.org/A189673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A189673: Fixed point of the morphism 0→010, 1→110.
    a(1) = 0
    for n > 1:
        if n % 3 == 0  → a(n) = 0
        if n % 3 == 2  → a(n) = 1
        if n % 3 == 1  → a(n) = a((n + 2) / 3)
*/
seq_t a(idx_t n)
{
    /* Handle the trivial base case (offset = 1) */
    if (n <= 1) return (seq_t)0;

    switch (n % 3) {
        case 0:  /* 3*k */
            return (seq_t)0;
        case 2:  /* 3*k - 1 */
            return (seq_t)1;
        case 1:  /* 3*k - 2, reduce to a smaller index */
            return a((n + 2) / 3);
    }
    /* Should never reach here */
    return (seq_t)0;
}
