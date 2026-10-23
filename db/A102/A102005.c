/*
 * A102005  Fixed point of the morphism 1 -> 12, 2 -> 111.
 * https://oeis.org/A102005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   fixed point of morphism 1 → 12, 2 → 111
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precompute lengths of the expansion of 1 after k iterations
       until it exceeds the requested index.  The lengths satisfy
       a[0] = 1, a[1] = 2, a[k] = a[k-1] + 3*a[k-2]. */
    static seq_t LEN[128] = {0};
    static int LEN_SZ = 0;

    if (LEN_SZ == 0) {
        LEN[0] = 1;
        LEN[1] = 2;
        LEN_SZ = 2;
    }

    /* extend LEN until the last entry > n */
    while (LEN[LEN_SZ - 1] <= (seq_t)n) {
        LEN[LEN_SZ] = LEN[LEN_SZ - 1] + 3 * LEN[LEN_SZ - 2];
        ++LEN_SZ;
    }

    /* find the smallest k with LEN[k] > n */
    int k = LEN_SZ - 1;

    /* walk the expansion tree backwards, keeping the current
       symbol (1 or 2).  At level k we are looking for the
       character that appears after k substitutions of the root
       symbol 1. */
    seq_t symbol = 1;          /* current symbol in the tree */
    while (k > 0) {
        seq_t len1_prev = LEN[k - 1];        /* expansion of 1 at level k-1 */
        seq_t len2_prev = 3 * len1_prev;    /* expansion of 2 at level k-1 */

        if (symbol == 1) {
            if ((seq_t)n < len1_prev) {
                /* goes into the first child (symbol 1) */
                symbol = 1;
            } else {
                n -= (idx_t)len1_prev;
                symbol = 2;
            }
        } else {  /* symbol == 2 */
            /* expansion of 2 is 1,1,1 each of length len1_prev */
            if ((seq_t)n < len1_prev) {
                /* first child */
                symbol = 1;
            } else if ((seq_t)n < 2 * len1_prev) {
                /* second child */
                n -= (idx_t)len1_prev;
                symbol = 1;
            } else {
                /* third child */
                n -= 2 * (idx_t)len1_prev;
                symbol = 1;
            }
        }
        --k;
    }

    return symbol;   /* 1 or 2 */
}
