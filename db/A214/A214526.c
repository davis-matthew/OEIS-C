/*
 * A214526  Manhattan distances between n and 1 in a square spiral with
 *          positive integers and 1 at the center.
 * https://oeis.org/A214526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = layer + abs(((n-1) mod (2*layer)) - layer), where layer =
 *            ( floor(sqrt(n-1)) + 1 ) / 2. The function works for any
 *            unsigned idx_t (uint64_t) and any seq_t (uint8_t … unsigned
 *            __int128, or arbitrary-precision)
 * Range:     u8 n <= 8673, u16 n <= 8673, u32 n <= 8673, u64 n <= 8673,
 *            u128 n <= 8673, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square-root for 64-bit unsigned integers: floor(sqrt(n)). */
static idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    for (int i = 63; i >= 0; --i) {
        idx_t bit = 1ULL << i;
        /* Test whether setting this bit keeps (res|bit)^2 <= n.
           Avoid overflow by dividing: (res|bit) <= n/(res|bit). */
        if ((res | bit) <= n / (res | bit))
            res |= bit;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Compute the layer of n in the spiral. */
    idx_t layer = (isqrt_u64(n - 1) + 1) / 2;

    /* For n=1, layer=0, distance is 0. */
    if (layer == 0)
        return 0;

    idx_t side   = 2 * layer;                      /* length of a ring side */
    idx_t offset = (n - 1) % side;                /* position along the ring */
    idx_t diff   = offset >= layer ? offset - layer : layer - offset;

    /* The Manhattan distance fits in seq_t because it is bounded by
       roughly sqrt(n)/2.  Cast after the arithmetic to avoid overflow. */
    return (seq_t)(layer + diff);
}
