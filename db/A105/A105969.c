/*
 * A105969  Fixed point of the morphism 1 -> 123, 2 -> 212, 3 -> 345, 4 ->
 *          434, 5 -> 561, 6 -> 656, starting from a(0) = 1.
 * https://oeis.org/A105969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of the morphism:
 *   1 → 123
 *   2 → 212
 *   3 → 345
 *   4 → 434
 *   5 → 561
 *   6 → 656
 * a(0) = 1
 *
 * The n-th term is obtained by writing n in base 3 and following
 * the corresponding branch in the substitution tree.
 */
#include <stdint.h>

static const unsigned char trans[7][3] = {
    {0,0,0},           /* unused index 0 */
    {1,2,3},           /* 1 → 1 2 3   */
    {2,1,2},           /* 2 → 2 1 2   */
    {3,4,5},           /* 3 → 3 4 5   */
    {4,3,4},           /* 4 → 4 3 4   */
    {5,6,1},           /* 5 → 5 6 1   */
    {6,5,6}            /* 6 → 6 5 6   */
};

seq_t a(idx_t n) {
    /* Handle n == 0 implicitly: stack empty keeps s = 1 */
    int s = 1;
    uint64_t t = (uint64_t)n;   /* use 64‑bit to avoid overflow on division */
    int digits[65];
    int top = 0;

    while (t > 0) {
        digits[top++] = (int)(t % 3);
        t /= 3;
    }

    for (int i = top - 1; i >= 0; --i) {
        s = trans[s][digits[i]];
    }

    return (seq_t)s;
}
