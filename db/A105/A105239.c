/*
 * A105239  Trajectory of 1 under the morphism 1 -> 121, 2 -> 434, 3 -> 212, 4
 *          -> 343.
 * https://oeis.org/A105239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) as the nth symbol of the fixed point of the morphism
    1 → 121, 2 → 434, 3 → 212, 4 → 343 (trajectory of 1).
    The algorithm uses the base‑3 representation of the index n:
    starting from the root symbol 1, for each base‑3 digit (most
    significant first) we descend one level of the substitution tree
    by applying the appropriate substitution.
*/
static const seq_t _a105239_map[5][3] = {
    {0,0,0},        /* unused 0 */
    {1,2,1},        /* 1 → 121 */
    {4,3,4},        /* 2 → 434 */
    {2,1,2},        /* 3 → 212 */
    {3,4,3}         /* 4 → 343 */
};

seq_t a(idx_t n) {
    /* Determine the number of levels needed to cover position n.
       For n == 0 we need at least one level (depth 1). */
    int depth = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        tmp /= 3;
        depth++;
    }
    if (depth == 0) depth = 1;          /* n == 0 case */

    /* Extract base‑3 digits of n, most significant first. */
    int digits[depth];
    tmp = n;
    for (int i = depth - 1; i >= 0; --i) {
        digits[i] = (int)(tmp % 3);
        tmp /= 3;
    }

    /* Traverse the substitution tree according to the digits. */
    seq_t symbol = 1;                     /* root symbol */
    for (int i = 0; i < depth; ++i) {
        int d = digits[i];
        symbol = _a105239_map[(int)symbol][d];
    }
    return symbol;
}
