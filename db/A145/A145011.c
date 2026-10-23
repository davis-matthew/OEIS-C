/*
 * A145011  First differences of A007775.
 * https://oeis.org/A145011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*((abs(abs(((n-1) mod 8)-3)-1) mod 3)+1). For integer n
 *            ≥ 1 this yields the 8‑periodic sequence 6,4,2,4,2,4,6,2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* (n-1) mod 8 is obtained by masking with 7.
       For n = 0 the result is undefined, but the offset is 1. */
    unsigned r = (unsigned)((n - 1) & 7ULL);

    /* Periodic table for the 8 residues of (n-1) mod 8. */
    static const seq_t table[8] = {
        (seq_t)6,  /* r = 0  → a(n)=6   (n=1) */
        (seq_t)4,  /* r = 1  → a(n)=4   (n=2) */
        (seq_t)2,  /* r = 2  → a(n)=2   (n=3) */
        (seq_t)4,  /* r = 3  → a(n)=4   (n=4) */
        (seq_t)2,  /* r = 4  → a(n)=2   (n=5) */
        (seq_t)4,  /* r = 5  → a(n)=4   (n=6) */
        (seq_t)6,  /* r = 6  → a(n)=6   (n=7) */
        (seq_t)2   /* r = 7  → a(n)=2   (n=8) */
    };

    return table[r];
}
