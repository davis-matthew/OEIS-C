/*
 * A021559  Decimal expansion of 1/555.
 * https://oeis.org/A021559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = (n+2)^3 mod 9 (for n > 0, a(0)=0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    /* Compute (n+2) modulo 3.  We use idx_t to avoid overflow. */
    idx_t r = (n + 2) % 3;
    /* r ∈ {0,1,2}.  The cube modulo 9 is 
       0^3≡0, 1^3≡1, 2^3≡8 (mod 9). */
    switch (r) {
        case 0: return (seq_t)0;
        case 1: return (seq_t)1;
        case 2: return (seq_t)8;
    }
    /* Unreachable, but keep compiler happy. */
    return 0;
}
