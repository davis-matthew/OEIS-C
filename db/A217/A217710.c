/*
 * A217710  Cardinality of the set of possible heights of AVL trees with n
 *          (leaf-) nodes.
 * https://oeis.org/A217710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 2635, u32 n <= 9465, u64 n <= 9465,
 *            u128 n <= 9465, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = |{ h ≥ 0 : F_{h+2} ≤ n ≤ 2^h }|
 * where F_k is the k-th Fibonacci number (F_0 = 0, F_1 = 1).
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1; guard for n = 0 */
        return 0;

    seq_t N = (seq_t)n;    /* convert index to seq_t for arithmetic */

    seq_t count = 0;       /* number of admissible heights */

    /* Fibonacci variables:
     * f0  = F_{h+2}  (minimal leaf count for current height h)
     * f1  = F_{h+3}  (next Fibonacci value)
     */
    seq_t f0 = (seq_t)1;   /* F_2  = 1  (h = 0) */
    seq_t f1 = (seq_t)2;   /* F_3  = 2  (h = 1) */

    seq_t maxLeaves = (seq_t)1;   /* 2^h  for current height h (h = 0) */

    for (;;)
    {
        if (f0 > N)            /* F_{h+2} exceeds n → no more heights possible */
            break;

        if (N <= maxLeaves)    /* n lies within the bounds for this height */
            ++count;

        /* Advance to the next height (h + 1) */
        seq_t tmp = f1;        /* store F_{h+3} before updating */
        f1 = f0 + f1;          /* compute F_{h+4} for the following step */
        f0 = tmp;              /* now f0 = F_{h+3} → minimal leaves for h+1 */

        maxLeaves <<= 1;        /* 2^(h+1) for the next height */
    }

    return count;
}
