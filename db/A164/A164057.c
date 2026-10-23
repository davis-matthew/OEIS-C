/*
 * A164057  Complement to A164056, change A164056 bits (0->1; 1->0). Provides
 *          a coding template for Petoukhov matrices, relating to DNA codons.
 * https://oeis.org/A164057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 if popcount(gr(k)) < popcount(gr(k-1)),
 *                else 0, where gr(k) = k ^ (k >> 1) is the
 *                n‑bit Gray code term and k = n - (2^r - 1)
 *                (r = ⌊log₂(n+1)⌋).  The first element of each
 *                2^r block (k = 0) is defined to be 1.
 * This reproduces OEIS A164057 (the complement of A164056).
 */

#include <stdint.h>
#include <stdbool.h>

/* floor(log2(x)) for non‑zero 64‑bit unsigned integers.
   For x == 0 we define the result to be 63, which is the
   value of ⌊log₂(UINT64_MAX)⌋. */
static inline idx_t
floor_log2_uint64(idx_t x)
{
    return x == 0 ? 63 : 63 - __builtin_clzll(x);
}

seq_t a(idx_t n)
{
    /* Determine the row r such that 2^r - 1 <= n < 2^(r+1) - 1 */
    idx_t r = floor_log2_uint64(n + 1);
    idx_t offset = (1ULL << r) - 1;     /* start index of this row   */
    idx_t k = n - offset;              /* position within the row   */

    /* Gray code of k */
    uint64_t g = k ^ (k >> 1);

    if (k == 0)            /* first element of the row      */
        return (seq_t)1;

    uint64_t gprev = (k - 1) ^ ((k - 1) >> 1);

    /* Count the ones in the two Gray code terms */
    unsigned pc_g   = __builtin_popcountll(g);
    unsigned pc_gp  = __builtin_popcountll(gprev);

    return (seq_t)((pc_g < pc_gp) ? 1 : 0);
}
