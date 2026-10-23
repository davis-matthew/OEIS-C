/*
 * A194917  Fractalization of (n-[nr-n]), where [ ]=floor and r=(1+sqrt(5))/2
 *          (the golden ratio).
 * https://oeis.org/A194917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A194917: Fractalization of (n - ⌊n·r - n⌋),  r = (1+√5)/2
 * The sequence is obtained by reading a triangular array.  
 *   Row 1: [1]
 *   For r ≥ 2, take row (r‑1), insert the integer r at position 2r – ⌊r·r⌊(1+√5)/2⌋⌋
 *   (positions are 1‑based).  
 * The function returns the value at linear index n (offset 1). */
#include <stdint.h>

static const idx_t PHI_NUM = 618034ULL;   /* (φ‑1) ≈ 0.618033988…  ≈ 618034/10^6  */
static const idx_t PHI_DEN = 1000000ULL;

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* position within the row (1‑based) */
    idx_t pos = n - (r - 1) * r / 2;

    while (r > 1)
    {
        /* floor(r·φ) = r + floor(r·(φ‑1))  */
        idx_t floor_r_phi = r + ( ( (__int128)r * PHI_NUM ) / PHI_DEN );
        idx_t b = 2 * r - floor_r_phi;          /* insertion position for r */

        if (pos == b) return (seq_t)r;          /* the element at this spot is r */
        if (pos > b) --pos;                     /* shift left of the inserted r */
        --r;
    }
    return (seq_t)1;
}
