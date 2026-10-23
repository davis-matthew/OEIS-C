/*
 * A174354  a(0)=1, a(1)=a(2)=2, a(3)=8 and for n >= 4: if n == 0 (mod 4),
 *          a(n)=2, if n == 1 (mod 4), a(n)=8, if n == 2 (mod 4), a(n)=2, if n
 *          == 3 (mod 8), a(n)=32, if n = 16k + 15, a(n) = 128*4^k, and if n =
 *          16k+7, a(n) = 128*4^(k-1).
 * https://oeis.org/A174354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 94, u32 n <= 222, u64 n <= 478,
 *            u128 n <= 990, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174354: a(0)=1, a(1)=a(2)=2, a(3)=8.
   For n>=4:
     if n%4==0 or n%4==2 → 2
     if n%4==1           → 8
     if n%4==3:
        if n%8==3        → 32
        else if n%16==15 → 128*4^(n/16)
        else            → 128*4^(n/16-1)   (i.e. 32*4^(n/16))
   All arithmetic uses type seq_t, cast from idx_t. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)8;

    /* n >= 4 */
    switch ((uint64_t)(n & 3ULL))
    {
        default:                     /* n%4 == 0 or 2 */
            return (seq_t)2;
        case 1:                     /* n%4 == 1 */
            return (seq_t)8;
        case 3:                     /* n%4 == 3 */
            if ((n & 7ULL) == 3ULL) /* n%8 == 3 */
                return (seq_t)32;

            if ((n & 15ULL) == 15ULL)  /* n%16 == 15 */
            {
                uint64_t k = n >> 4;   /* n/16 */
                seq_t p = (seq_t)1;
                for (uint64_t i = 0; i < k; ++i) p *= (seq_t)4;
                return (seq_t)128 * p;
            }
            else                       /* n%16 == 7 */
            {
                uint64_t k = n >> 4;   /* n/16 */
                if (k == 0) return (seq_t)32; /* n==7 */
                seq_t p = (seq_t)1;
                for (uint64_t i = 0; i < k; ++i) p *= (seq_t)4;
                return (seq_t)32 * p;          /* 128*4^(k-1) */
            }
    }
}
