/*
 * A220468  Number of cyclotomic cosets of n mod 10.
 * https://oeis.org/A220468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by exploring the functional graph of multiplication by n
   modulo 10.  The result depends only on n mod 10, so we reduce n first
   and then count the distinct cycles in the directed graph on
   {0,…,9} defined by x → (x*n) mod 10. */
seq_t a(idx_t n)
{
    /* Reduce n modulo 10 once.  The product is done in seq_t type. */
    seq_t mult = (seq_t)(n % 10);

    unsigned char state[10] = {0};   /* 0 = unvisited, 1 = visiting,
                                      2 = finished */
    seq_t count = 0;

    for (idx_t i = 0; i < 10; ++i) {
        if (state[i] != 0) continue;   /* already processed */

        /* Traverse the path starting from i until we reach a
           visited node.  All indices are 0..9. */
        idx_t j = i;
        while (state[j] == 0) {
            state[j] = 1;
            j = (idx_t)(((seq_t)j * mult) % 10);
        }

        /* If we looped back to a node that is still in the current
           traversal, a new cycle has been found. */
        if (state[j] == 1) {
            ++count;
        }

        /* Mark all nodes in the path as finished. */
        j = i;
        while (state[j] == 1) {
            state[j] = 2;
            j = (idx_t)(((seq_t)j * mult) % 10);
        }
    }

    return count;
}
