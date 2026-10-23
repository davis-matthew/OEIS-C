/*
 * A014578  Binary expansion of Thue constant (or Roth's constant).
 * https://oeis.org/A014578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, mult
 * Formula:   a(0)=0; for n>0: if n%3==0 then a(n)=1-a(n/3), else a(n)=1
 * Range:     u8 n <= 9771, u16 n <= 9771, u32 n <= 9771, u64 n <= 9771,
 *            u128 n <= 9771, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    unsigned char flip = 0;
    while (n % 3 == 0) {
        n /= 3;
        flip ^= 1;
    }

    return flip ? (seq_t)0 : (seq_t)1;
}
