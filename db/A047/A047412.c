/*
 * A047412  Numbers that are congruent to {0, 1, 2, 4, 6} mod 8.
 * https://oeis.org/A047412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047412:  numbers congruent to {0,1,2,4,6} modulo 8.
   Formula used:
        a(n) = 8*floor((n-1)/5) + f((n-1)%5) ,
        with f(0)=0, f(1)=1, f(2)=2, f(3)=4, f(4)=6.
   a(1) = 0.  The function is correct for any n ≥ 1 and is
   independent of any previous calls. */

seq_t a(idx_t n)
{
    if (n == 0)               /* safety for out-of-range calls */
        return (seq_t)0;

    idx_t q = (n - 1) / 5;    /* number of complete 5‑term blocks before n */
    idx_t r = (n - 1) % 5;    /* position inside the next block (0‑4) */

    seq_t base = (seq_t)q * (seq_t)8;   /* 8 * q */

    /* Add the appropriate offset according to r. */
    switch (r) {
        case 0: return base;          /* +0 */
        case 1: return base + 1;      /* +1 */
        case 2: return base + 2;      /* +2 */
        case 3: return base + 4;      /* +4 */
        case 4: return base + 6;      /* +6 */
    }

    /* Should never reach here, but return base as a fallback. */
    return base;
}
