/*
 * A101668  Fixed point of morphism 0 -> 01, 1 -> 20, 2 -> 00.
 * https://oeis.org/A101668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth symbol of the fixed point of the morphism φ: 0 →
 *            01, 1 → 20, 2 → 00, with a(0)=0. The sequence can be obtained by
 *            starting with symbol 0 and, for every bit of n from the
 *            most‑significant to the least‑significant, following φ’s first
 *            letter when the bit is 0 and its second letter w...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t first[3]  = { 0, 2, 0 };
static const seq_t second[3] = { 1, 0, 0 };

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t x = 0;                  /* current symbol */
    int msb = 63 - __builtin_clzll((unsigned long long)n);
    idx_t mask = ((idx_t)1) << msb;

    for (; mask; mask >>= 1) {
        if (n & mask)
            x = second[(int)x];
        else
            x = first[(int)x];
    }
    return x;
}
