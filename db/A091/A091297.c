/*
 * A091297  A fixed point of the morphism 0 -> 02, 1 -> 02, 2 -> 11, starting
 *          from 0.
 * https://oeis.org/A091297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) defined by the morphism 0→02, 1→02, 2→11; a(1)=0
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)                 /* base case: the initial symbol */
        return 0;

    /* parent index (ceil(n/2)) without overflow */
    idx_t parent = (n >> 1) + (n & 1);

    /* whether we are the right child of the parent (0 for left, 1 for right) */
    int offset = (int)((n - 1) & 1);

    seq_t p = a(parent);       /* symbol of the parent */

    /* apply the morphism: 0,1 → left:0/right:2 ; 2 → left:1/right:1 */
    if (p == 0 || p == 1)
        return offset ? 2 : 0;
    else                          /* p == 2 */
        return 1;
}
