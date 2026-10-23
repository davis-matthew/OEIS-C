/*
 * A100261  Continued fraction expansion of cot(1-Pi/4).
 * https://oeis.org/A100261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continuous fraction expansion of cot(1−π/4).
   a(1)=4, a(2)=1, a(3)=1.
   For n ≥ 4 let
     p = n − 4,
     k = p / 6         (integer division),
     r = p % 6.
   Then
     r = 0 → a(n)=2
     r = 1 → a(n)=3·k+2
     r = 2 → a(n)=1
     r = 3 → a(n)=12·k+16
     r = 4 → a(n)=1
     r = 5 → a(n)=3·k+4
*/

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;
    if (n < 1)  return (seq_t)0;       /* undefined for n < 1, return 0 */
    idx_t p = n - 4;
    idx_t k = p / 6;
    idx_t r = p % 6;
    switch (r) {
        case 0:  return (seq_t)2;
        case 1:  return (seq_t)(3ULL * (seq_t)k + 2);
        case 2:  return (seq_t)1;
        case 3:  return (seq_t)(12ULL * (seq_t)k + 16);
        case 4:  return (seq_t)1;
        case 5:  return (seq_t)(3ULL * (seq_t)k + 4);
        default: return (seq_t)0;   /* never reached */
    }
}
