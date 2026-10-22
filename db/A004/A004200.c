/*
 * A004200  Continued fraction for Sum_{k>=0} 1/3^(2^k).
 * https://oeis.org/A004200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   Recurrence with base cases a(0)=0, a(1)=2, a(2)=5, and reduction
 *            rules based on residue classes modulo 8, 16, and 32
 * Range:     u8 n <= 9303, u16 n <= 9303, u32 n <= 9303, u64 n <= 9303,
 *            u128 n <= 9303, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 2;
    if (n == 2) return 5;
    
    idx_t r32 = n % 32;
    idx_t r16 = n % 16;
    idx_t r8 = n % 8;
    
    /* Check modulo 32 patterns */
    if (r32 == 8 || r32 == 25) return 3;
    if (r32 == 9 || r32 == 24) return 1;
    
    /* Check modulo 16 patterns */
    if (r16 == 4 || r16 == 13) return 3;
    if (r16 == 5 || r16 == 12) return 1;
    
    /* Check modulo 8 patterns */
    if (r8 == 2 || r8 == 7) return 5;
    if (r8 == 3 || r8 == 6) return 3;
    
    /* Recursive reduction rules */
    if (r16 == 0) return a(n / 2);
    if (r16 == 1) return a((n + 1) / 2);
    
    return 0;
}
