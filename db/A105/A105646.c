/*
 * A105646  Fixed point of the morphism 1 -> 121, 2 -> 343, 3 -> 434, 4 ->
 *          212, starting from a(0) = 1.
 * https://oeis.org/A105646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = symbol at position n of the fixed point of the morphism *
 *            1 → 121, 2 → 343, 3 → 434, 4 → 212, * starting from a(0) = 1
 *            (0‑based indexing)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for NULL, if needed */

static const seq_t MAP[5][3] = {
    {0, 0, 0},                                   /* unused index 0   */
    {(seq_t)1, (seq_t)2, (seq_t)1},              /* 1 → 121           */
    {(seq_t)3, (seq_t)4, (seq_t)3},              /* 2 → 343           */
    {(seq_t)4, (seq_t)3, (seq_t)4},              /* 3 → 434           */
    {(seq_t)2, (seq_t)1, (seq_t)2}               /* 4 → 212           */
};

seq_t a(idx_t n)
{
    if (n == 0)                     /* base word is just "1" */
        return (seq_t)1;

    /* Recurse to the parent symbol that produced the current position. */
    seq_t parent = a(n / 3);
    idx_t off = n % 3;              /* 0,1,2 for the 3‑symbol block */

    /* parent is a small integer (1..4).  Convert it to an int for indexing. */
    int p = (int)parent;
    return MAP[p][off];
}
