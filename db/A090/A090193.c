/*
 * A090193  a(n) = A053838(n) + 1 modulo 3.
 * https://oeis.org/A090193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = A053838(n)+1 mod 3, where the infinite word is the
   fixed point of the morphism 0→012, 1→120, 2→201 (starting from 0). */
#include <stdint.h>

static const uint8_t map[3][3] = {
    {0, 1, 2},  /* 0 → 012 */
    {1, 2, 0},  /* 1 → 120 */
    {2, 0, 1}   /* 2 → 201 */
};

seq_t a(idx_t n)
{
    uint8_t digit = 0;            /* A053838(0) = 0 */
    uint8_t rems[64];             /* enough for 64-bit n (log₃(2⁶⁴)< 40) */
    int depth = 0;

    /* Collect remainders while descending the tree */
    while (n > 0) {
        rems[depth++] = n % 3;    /* remainder in base 3 */
        n /= 3;
    }

    /* Descend back from the root applying the morphism */
    for (int i = depth - 1; i >= 0; --i) {
        uint8_t r = rems[i];
        digit = map[digit][r];
    }

    /* a(n) = (A053838(n) + 1) mod 3 */
    uint8_t res = (digit + 1) % 3;
    return (seq_t)res;
}
