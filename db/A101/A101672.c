/*
 * A101672  Fixed point of morphism 0 -> 01, 1 -> 20, 2 -> 11.
 * https://oeis.org/A101672
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

/* A101672: fixed point of morphism 0→01, 1→20, 2→11.  a(0)=0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* transition table: trans[parent][bit] */
    static const unsigned int trans[3][2] = {
        {0, 1},   /* 0 → 0,1 */
        {2, 0},   /* 1 → 2,0 */
        {1, 1}    /* 2 → 1,1 */
    };

    unsigned int cur = 0;          /* start at the root symbol 0 */

    /* find the position of the most significant bit of n */
    idx_t t = n;
    int msb = 0;
    while (t > 1) {
        t >>= 1;
        msb++;
    }

    /* traverse the bits of n from most to least significant */
    for (int i = msb; i >= 0; --i) {
        int bit = (int)((n >> i) & 1ULL);
        cur = trans[cur][bit];
    }

    return (seq_t)cur;
}
