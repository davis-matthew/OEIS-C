/*
 * A014577  The regular paper-folding sequence (or dragon curve sequence).
 *          Alphabet {1,0}.
 * https://oeis.org/A014577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>0, if n is even, a(n)=1 when n%4==0, otherwise a(n)=0;
   if n is odd, a(n)=a((n-1)/2) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    while (n != 0) {
        if (n & 1U) {          /* odd */
            n >>= 1;
        } else {               /* even */
            if ((n & 3U) == 0U)  /* n ≡ 0 (mod 4) */
                return (seq_t)1;
            else                 /* n ≡ 2 (mod 4) */
                return (seq_t)0;
        }
    }
    return (seq_t)1;          /* reached n == 0 */
}
