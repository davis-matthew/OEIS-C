/*
 * A103127  Numbers congruent to {-1, 1, 3, 5} mod 16.
 * https://oeis.org/A103127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*floor((n-1)/4) + {1,3,5,15} for (n-1)%4 = 0,1,2,3
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* sequence starts at n = 1 (offset 1) */

    /* Compute block index q and remainder r */
    idx_t q = (n - 1) / 4;
    unsigned r = (unsigned)((n - 1) % 4);

    /* Base value for the current 16‑number block */
    seq_t base = (seq_t)q * (seq_t)16;

    /* Offsets within each block */
    static const seq_t offsets[4] = {1, 3, 5, 15};

    return base + offsets[r];
}
