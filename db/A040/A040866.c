/*
 * A040866  Continued fraction for sqrt(896).
 * https://oeis.org/A040866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040866: Continued fraction of sqrt(896). 
   a(0)=29; for n>0,
   a(n) = 1 if (n-1)%4==0; 13 if (n-1)%4==1; 1 if (n-1)%4==2; 58 if (n-1)%4==3. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)29;

    idx_t r = (n - 1) & 3;   /* (n-1) modulo 4 (bitwise since 4 = 2^2) */
    switch (r) {
    case 0: return (seq_t)1;
    case 1: return (seq_t)13;
    case 2: return (seq_t)1;
    case 3: return (seq_t)58;
    }
    return (seq_t)0;   /* unreachable */
}
