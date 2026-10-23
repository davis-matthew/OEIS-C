/*
 * A040265  Continued fraction for sqrt(282).
 * https://oeis.org/A040265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040265: continued fraction for sqrt(282).
   Definition: a(0)=16; for n>0 let e=v2(n).
   if e==0 then a(n)=1
   if e==1 then a(n)=3
   if e==2 then a(n)=4
   if e>=3 then a(n)=32
   (equivalently, a(n) depends only on the exponent of 2 in n). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)16;
    idx_t temp = n;
    unsigned e = 0;
    while ((temp & 1ULL) == 0) {
        e++;
        temp >>= 1;
    }
    if (e == 0) return (seq_t)1;
    if (e == 1) return (seq_t)3;
    if (e == 2) return (seq_t)4;
    return (seq_t)32;
}
