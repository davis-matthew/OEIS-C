/*
 * A118766  A118764(A118764(n)).
 * https://oeis.org/A118766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A118766: Permutation that alternates a cyclic shift of the units digit by +2
   on odd-numbered decile blocks with a modified shift on even-numbered blocks:
   • For n < 10, a(n) = n.
   • For a decile block k = floor(n/10):
       – If k is odd: a(n) = 10*k + ((n%10 + 2) % 10).
       – If k is even (k ≥ 2):
           – If the units digit d = 9: a(n) = n (fixed point).
           – If d is even:
                * If d = 8: a(n) = 10*k + 1.
                * Else: a(n) = 10*k + d + 2.
           – If d is odd:
                * If d = 7: a(n) = 10*k.
                * Else: a(n) = 10*k + d + 2. */

seq_t a(idx_t n)
{
    if (n < 10)
        return (seq_t)n;

    seq_t k = n / 10;
    seq_t d = n % 10;

    if (k & 1) {                     /* odd block */
        return k * 10 + (d + 2) % 10;
    } else {                          /* even block (k ≥ 2) */
        if (d == 9)
            return (seq_t)n;          /* fixed point 9 */
        if ((d & 1) == 0) {          /* even d */
            if (d == 8)
                return k * 10 + 1;    /* 8 → 1 */
            return k * 10 + d + 2;   /* d → d + 2 */
        } else {                     /* odd d */
            if (d == 7)
                return k * 10;        /* 7 → 0 */
            return k * 10 + d + 2;   /* d → d + 2 */
        }
    }
}
