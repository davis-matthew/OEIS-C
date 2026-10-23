/*
 * A100260  Fixed point of morphism 0 -> 01, 1 -> 02, 2 -> 31, 3 -> 32.
 * https://oeis.org/A100260
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

/* Recurrence: a(1)=0, a(2n)=second[a(n)], a(2n-1)=first[a(n)] */

#include <stdint.h>

static const uint8_t first_map[4] = {0, 0, 3, 3};
static const uint8_t second_map[4] = {1, 2, 1, 2};

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n<1; not used in tests */
    if (n == 1)
        return 0;          /* a(1)=0 */

    /* compute parent index as ceil(n/2) without overflow */
    idx_t parent_idx = (n >> 1) + (n & 1);
    seq_t parent = a(parent_idx);  /* recursion on smaller index */

    /* parent is guaranteed 0..3; convert to unsigned for array indexing */
    unsigned idx = (unsigned)parent;

    return (n & 1) ? (seq_t)first_map[idx] : (seq_t)second_map[idx];
}
