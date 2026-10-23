/*
 * A105778  Trajectory of 1 under the morphism 1->{1,2,1,2,1}, 2->{4,3,4,3,4},
 *          3->{2,1,1,1,2}, 4->{3,4,3,4,3}.
 * https://oeis.org/A105778
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

/* 
 * Sequence A105778: trajectory of 1 under the morphism
 *   1 → [1,2,1,2,1]
 *   2 → [4,3,4,3,4]
 *   3 → [2,1,1,1,2]
 *   4 → [3,4,3,4,3]
 * 
 * a(0) is the first symbol of the fixed point (1),
 * and a(n) is the symbol at index n (0‑based).
 */

seq_t a(idx_t n)
{
    /* Special case for the very first symbol. */
    if (n == 0)
        return (seq_t)1;

    /* Mapping table for the morphism. */
    static const unsigned char m[4][5] = {
        {1, 2, 1, 2, 1},    /* φ(1) */
        {4, 3, 4, 3, 4},    /* φ(2) */
        {2, 1, 1, 1, 2},    /* φ(3) */
        {3, 4, 3, 4, 3}     /* φ(4) */
    };

    /* Convert n to base‑5 digits (least significant first). */
    idx_t t = n;
    int digits[64];
    int len = 0;
    while (t > 0) {
        digits[len++] = (int)(t % 5);
        t /= 5;
    }

    /* Starting symbol 1 has index 0 in m. */
    unsigned char cur = 0;   /* 0 → symbol 1, 1 → symbol 2, etc. */

    /* Traverse the digits from most significant to least. */
    for (int i = len - 1; i >= 0; --i) {
        unsigned int d = (unsigned int)digits[i];   /* 0 … 4 */
        cur = m[cur][d] - 1;                       /* new symbol index */
    }

    /* Return the found symbol as seq_t. */
    return (seq_t)(cur + 1);
}
