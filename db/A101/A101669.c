/*
 * A101669  Fixed point of morphism 0 -> 01, 1 -> 20, 2 -> 01.
 * https://oeis.org/A101669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101669: a(n) is the nth term of the fixed point of the
   morphism 0→01, 1→20, 2→01, starting from the symbol 0. */

seq_t a(idx_t n)
{
    /* Base case: the fixed point begins with 0. */
    if (n == 0) return 0;

    /* Recursive step: the current symbol is derived from its parent
       symbol (at index n/2).  Let p be the parent symbol.  The
       morphism maps:
           0 or 2  -> 0,1
           1       -> 2,0
       The choice between the two children depends on the least
       significant bit of n (0 → first child, 1 → second child). */
    seq_t p = a(n >> 1);
    idx_t bit = n & 1;            /* 0 if even, 1 if odd */

    /* Map parent symbol and bit to the new symbol. */
    return (p == 1) ? (bit ? 0 : 2)   /* parent = 1 */
                    : (bit ? 1 : 0);  /* parent = 0 or 2 */
}
