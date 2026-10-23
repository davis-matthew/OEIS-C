/*
 * A160529  a(1) = 1; for n>1, a(n) = a(n-1) + d1 + d2 where d1 = 4 if n is
 *          even. d1 = 1 if n is odd, d2 = 15 if n mod 4 = 0, d2 = 0 if n mod
 *          4 != 0.
 * https://oeis.org/A160529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*floor((n-1)/4) + [25,1,5,6] (n mod 4)
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Matches a(4k+1)=1+25k, a(4k+2)=5+25k, a(4k+3)=6+25k, a(4k+4)=25+25k) */

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n=0; return 0 or guard */
        return 0;

    idx_t q = (n - 1) / 4;          /* floor((n-1)/4) */
    seq_t base = (seq_t)q * 25;     /* common multiple of 25 */

    switch (n % 4) {
        case 0:  return base + 25; /* n ≡ 0 (mod 4)  → a = 25 + 25·floor((n-1)/4) */
        case 1:  return base + 1;  /* n ≡ 1 (mod 4)  → a = 1  + 25·floor((n-1)/4) */
        case 2:  return base + 5;  /* n ≡ 2 (mod 4)  → a = 5  + 25·floor((n-1)/4) */
        case 3:  return base + 6;  /* n ≡ 3 (mod 4)  → a = 6  + 25·floor((n-1)/4) */
    }
    /* unreachable, but keep compiler happy */
    return 0;
}
