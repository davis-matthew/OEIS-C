/*
 * A100337  Arshon's sequence with a different start: start from 3 and replace
 *          the letters in odd positions using 1 -> 123, 2 -> 231, 3 -> 312
 *          and the letters in even positions using 1 -> 321, 2-> 132, 3 ->
 *          213.
 * https://oeis.org/A100337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = symbol at position n in the infinite word obtained by
 *            starting with the word "3" and iteratively replacing each symbol
 *            by a length‑3 block that depends on the parity of its position:
 *            odd positions use 1→123, 2→231, 3→312; even positions use 1→321,
 *            2→132, 3→213
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the very first symbol (offset 1) */
    if (n == (idx_t)1)
        return (seq_t)3;

    /* Build the base‑3 digits of n‑1 (most significant digit last). */
    int digits[70];
    int len = 0;
    idx_t tmp = n - (idx_t)1;
    do
    {
        digits[len++] = (int)(tmp % (idx_t)3);
        tmp /= (idx_t)3;
    } while (tmp != (idx_t)0);

    /* Mapping tables: mapping[parity][symbol][offset] → child symbol.
       Parity: 0 = even, 1 = odd; symbols 1‑3; offsets 0‑2. */
    static const unsigned char mapping[2][4][3] = {
        /* even parity (0) */
        {
            {0, 0, 0},   /* dummy for symbol 0 */
            {3, 2, 1},   /* 1 → 321 → [3,2,1] */
            {1, 3, 2},   /* 2 → 132 → [1,3,2] */
            {2, 1, 3}    /* 3 → 213 → [2,1,3] */
        },
        /* odd parity (1) */
        {
            {0, 0, 0},   /* dummy for symbol 0 */
            {1, 2, 3},   /* 1 → 123 → [1,2,3] */
            {2, 3, 1},   /* 2 → 231 → [2,3,1] */
            {3, 1, 2}    /* 3 → 312 → [3,1,2] */
        }
    };

    /* Start from the root symbol 3 at index 1 (odd). */
    int currentSymbol = 3;
    idx_t currentIndex = (idx_t)1;
    int parity = 1;  /* odd */

    /* Traverse the path defined by the base‑3 digits, from most to least
       significant. */
    for (int i = len - 1; i >= 0; --i)
    {
        int offset = digits[i];  /* 0, 1, or 2 */

        /* Determine the child symbol using the parent parity and symbol. */
        currentSymbol = mapping[parity][currentSymbol][offset];

        /* Compute the child's index in the next level. */
        unsigned __int128 nextIndex =
            ((unsigned __int128)(currentIndex - (idx_t)1)) * (unsigned __int128)3
            + (unsigned __int128)offset + (unsigned __int128)1;
        currentIndex = (idx_t)nextIndex;

        /* Update parity for the next step. */
        parity = (currentIndex & (idx_t)1) ? 1 : 0;
    }

    return (seq_t)currentSymbol;
}
