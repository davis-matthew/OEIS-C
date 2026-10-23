/*
 * A174139  Numbers congruent to {0,1,2,3,4,10,11,12,13,14,20,21,22,23,24} mod
 *          25.
 * https://oeis.org/A174139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n-1) % 15) + 5 * ((((n-1) % 15) / 5)) + 25 * (((n-1) /
 *            15))
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174139: numbers congruent to {0,1,2,3,4,10,11,12,13,14,20,21,22,23,24} modulo 25. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not an official term, but safe guard */
    idx_t n1 = n - 1;                      /* zero‑based index */
    idx_t t   = n1 / 15;                   /* how many full 15‑term blocks */
    idx_t k   = n1 % 15;                   /* position inside the block (0‑14) */
    idx_t q   = k / 5;                     /* 0 for residues 0‑4, 1 for 5‑9, 2 for 10‑14 */
    idx_t r   = k % 5;                     /* intra‑group offset (0‑4) */

    seq_t val = (seq_t)q * (seq_t)10 +      /* 10*q gives base offset (0,10,20)   */
                (seq_t)r                   /* + r gives 0‑4 within each group  */
              + (seq_t)25 * (seq_t)t;       /* + 25*t for full 15‑term blocks    */

    return val;
}
