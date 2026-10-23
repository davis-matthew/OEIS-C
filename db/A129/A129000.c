/*
 * A129000  Start with an integer (in this case, 1). First, add 5 or 8 if the
 *          integer is odd or even, respectively. Then divide by 2.
 * https://oeis.org/A129000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1001, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129000: a(1)=1; thereafter
   if a(n‑1) is odd: a(n) = (a(n‑1)+5)/2
   if a(n‑1) is even: a(n) = (a(n‑1)+8)/2
   (equivalently a(n) = (a(n‑1)+((a(n‑1)&1)?5:8))>>1 )   */
seq_t a(idx_t n)
{
    /* indices start at 1; behaviour for n<1 is irrelevant */
    if (n < 2)      /* n==1 or lower */
        return 1;   /* a(1) = 1 */

    seq_t prev = 1; /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* choose the addend depending on parity of prev */
        seq_t add = (prev & 1) ? 5 : 8;
        /* next term: (prev + add)/2  using bitshift for division by 2 */
        prev = (prev + add) >> 1;
    }

    return prev;
}
