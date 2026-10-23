/*
 * A038138  Order of n (mod 7).
 * https://oeis.org/A038138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -(35*(n mod 7)^6 - 603*(n mod 7)^5 + 3860*(n mod 7)^4 * -
 *            11235*(n mod 7)^3 + 14465*(n mod 7)^2 - 6882*(n mod 7)) / 360
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   which reduces to the multiplicative order of n modulo 7
 *   (with a(0) = 0).  The sequence is 7‑periodic. */

static const seq_t order_mod7[7] = {0, 1, 3, 6, 3, 6, 2};

seq_t a(idx_t n)
{
    /* Modulo 7 selects the entry.  No overflow can occur because the
     * index type is unsigned and 7 fits in an int. */
    idx_t r = n % 7u;
    return order_mod7[r];
}
