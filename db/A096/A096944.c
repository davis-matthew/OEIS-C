/*
 * A096944  Seventh column of (1,5)-Pascal triangle A096940.
 * https://oeis.org/A096944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   bin_choose5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bin_choose5.h"
/*
 * Formula: a(n) = (n+30) * binomial(n+5,5) / 6
 * (Derived from a(n) = (n+30)*C(n+5,5)/6).
 */

seq_t a(idx_t n)
{
    /* C(n+5,5) */
    seq_t c = bin_choose5(n + 5);

    /* (n+30) * c / 6  – compute with 128‑bit intermediate if available. */
    seq_t mult = (seq_t)(n + 30);

#ifdef __SIZEOF_INT128__
    unsigned __int128 prod = (unsigned __int128)mult * (unsigned __int128)c;
    return (seq_t)(prod / 6);
#else
    /* Fallback: rely on built‑in multiplication (may overflow for very large n). */
    return (seq_t)((mult * c) / 6);
#endif
}
