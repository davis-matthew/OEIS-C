/*
 * A100336  Arshon's sequence with a different start: start from 2 and replace
 *          the letters in odd positions using 1 -> 123, 2 -> 231, 3 -> 312
 *          and the letters in even positions using 1 -> 321, 2-> 132, 3 ->
 *          213.
 * https://oeis.org/A100336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Arshon's sequence (A100336)
   a(1) = 2.
   For n > 1 let p = (n‑1)/3 + 1 (1‑based parent index), o = (n‑1)%3 (0,1,2).
   Let d = a(p).  If p is odd use odd_map[d][o], otherwise use even_map[d][o].
   odd_map: 1→123, 2→231, 3→312.
   even_map: 1→321, 2→132, 3→213.
*/
static const seq_t odd_map[4][3] = {
    {0, 0, 0},
    {1, 2, 3},   /* 1 → 123 */
    {2, 3, 1},   /* 2 → 231 */
    {3, 1, 2}    /* 3 → 312 */
};

static const seq_t even_map[4][3] = {
    {0, 0, 0},
    {3, 2, 1},   /* 1 → 321 */
    {1, 3, 2},   /* 2 → 132 */
    {2, 1, 3}    /* 3 → 213 */
};

static seq_t digit_rec(idx_t pos)
{
    if (pos == 1) {
        return (seq_t)2; /* base word "2" */
    }
    idx_t parent = (pos - 1) / 3 + 1;
    int  offset = (int)((pos - 1) % 3);

    seq_t parent_digit = digit_rec(parent);

    const seq_t (*map)[3] = ((parent & 1) ? odd_map : even_map);

    /* parent_digit is guaranteed to be 1, 2 or 3; cast to unsigned for array index */
    return map[(unsigned)parent_digit][offset];
}

seq_t a(idx_t n)
{
    return digit_rec(n);
}
