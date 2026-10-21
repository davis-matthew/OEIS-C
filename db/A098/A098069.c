/*
 * A098069  If C is a self-dual code over Z_4 of length N=24m+n, 1 <= n <= 24,
 *          then the minimum Lee weight of C is bounded above by 8m+a(n).
 * https://oeis.org/A098069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A098069 is periodic with period 24.  For n≥1,
   let r = ((n-1) mod 24)+1; then a(n) is given by the table below.
   The values are:
      r =  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12
     a(r)= 2,  2,  2,  4,  2,  4,  6,  8,  2,  4,  4,  4
      r = 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24
     a(r)= 4,  6,  6,  8,  8,  8,  6,  8,  8,  8, 10, 12 */
seq_t a(idx_t n)
{
    /* Compute remainder r in {1,…,24} */
    idx_t r = n % 24;
    if (r == 0) r = 24;

    /* Return the corresponding value */
    switch (r) {
        case 1: case 2: case 3: case 5: case 9: return (seq_t)2;
        case 4: case 6: case 10: case 11: case 12: case 13: return (seq_t)4;
        case 7: case 14: case 15: case 19: return (seq_t)6;
        case 8: case 16: case 17: case 18: case 20: case 21: case 22: return (seq_t)8;
        case 23: return (seq_t)10;
        case 24: return (seq_t)12;
    }
    /* Should never reach here; return 0 as fallback */
    return (seq_t)0;
}
