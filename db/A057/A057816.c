/*
 * A057816  If a(n) = 0 mod 3 then a(n+1)=a(n)/3 otherwise a(n+1)=4*a(n)+-1
 *          where sign is chosen to give a multiple of 3.
 * https://oeis.org/A057816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A057816
   Recurrence:
        a(1) = 43
        if a(n) ≡ 0 (mod 3) : a(n+1) = a(n)/3
        else if a(n) ≡ 1 (mod 3) : a(n+1) = 4*a(n) − 1   (makes multiple of 3)
        else (a(n) ≡ 2 (mod 3)) : a(n+1) = 4*a(n) + 1   (makes multiple of 3)
   The values may grow beyond 64 bits; seq_t may be arbitrary‑precision on some builds.
   This implementation follows the recurrence exactly. */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n < 1 */

    seq_t x = (seq_t)43;                /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t r = x % (seq_t)3;
        if (r == 0)        /* divisible by 3 */
        {
            x = x / (seq_t)3;
        }
        else if (r == 1)   /* a ≡ 1 (mod 3): use 4*a − 1 */
        {
            x = (seq_t)4 * x - (seq_t)1;
        }
        else              /* r == 2 : use 4*a + 1 */
        {
            x = (seq_t)4 * x + (seq_t)1;
        }
    }
    return x;
}
