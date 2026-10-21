/*
 * A083031  Numbers that are congruent to {0, 3, 7} mod 12.
 * https://oeis.org/A083031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*floor((n-1)/3) + {0,3,7}[ (n-1) mod 3 ]
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A083031: numbers congruent to {0,3,7} modulo 12. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1 – not used in tests */
    idx_t k = (n - 1) / 3;                /* floor((n-1)/3)   */
    idx_t r = (n - 1) % 3;                /* (n-1) mod 3       */
    seq_t base = (seq_t)12 * (seq_t)k;    /* 12 * floor((n-1)/3) */
    switch (r) {
        case 0: return base;              /* +0  when r==0 (n≡1 mod3) */
        case 1: return base + (seq_t)3;   /* +3  when r==1 (n≡2 mod3) */
        default: return base + (seq_t)7;  /* +7  when r==2 (n≡0 mod3) */
    }
}
