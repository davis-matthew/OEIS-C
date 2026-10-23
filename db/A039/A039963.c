/*
 * A039963  The period-doubling sequence A035263 repeated.
 * https://oeis.org/A039963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: The sequence is the fixed point of the morphism
 *   a -> ab ,  b -> cc ,  c -> ab
 * where a,b are represented by 1 and c by 0.
 * For any index n (0‑based) the nth term is obtained by
 * following the binary expansion of n from most significant
 * to least significant bit through the above morphism.
 * The algorithm runs in O(log n) time and uses only integer operations.
 */
seq_t a(idx_t n)
{
    /* Find the position of the most significant set bit. */
    unsigned msb = 0;
    idx_t t = n;
    while (t) {
        t >>= 1;
        ++msb;
    }
    /* n == 0  -> first element is 1 */
    if (msb == 0)
        return (seq_t)1;

    /* State: 0 = 'a' (1), 1 = 'b' (1), 2 = 'c' (0) */
    int state = 0;

    /* Transition tables for bit = 0 and bit = 1 */
    static const int next0[3] = {0, 2, 0};  /* a->a , b->c , c->a */
    static const int next1[3] = {1, 2, 1};  /* a->b , b->c , c->b */

    /* Walk the binary representation of n from the most significant
     * bit to the least significant bit. */
    for (int i = (int)msb - 1; i >= 0; --i) {
        int bit = (int)((n >> i) & 1);
        state = (bit ? next1[state] : next0[state]);
    }

    /* Map state to the actual term: states 0 and 1 correspond to 1,
     * state 2 corresponds to 0. */
    return (seq_t)(state == 2 ? 0 : 1);
}
