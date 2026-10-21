/*
 * A063924  Number of 3-dimensional cells in the regular 4-dimensional
 *          polytopes.
 * https://oeis.org/A063924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = A063925(n) - A063926(n) + A063927(n)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For the six regular convex 4‑polytopes: 5‑cell, hypercube, 16‑cell,
   24‑cell, 120‑cell, 600‑cell, the numbers of 3‑dimensional cells are
   5, 8, 16, 24, 120, 600.  The sequence is defined for n = 1..6.
   For n > 6 the function returns 0. */

seq_t a(idx_t n)
{
    /* Return the appropriate number of 3‑D cells. */
    switch ((idx_t)n) {
        case 1: return (seq_t)5;
        case 2: return (seq_t)8;
        case 3: return (seq_t)16;
        case 4: return (seq_t)24;
        case 5: return (seq_t)120;
        case 6: return (seq_t)600;
        default: return (seq_t)0;
    }
}
