/*
 * A057526  Number of applications of f to reduce n to 1, where f(k) is the
 *          integer among k/2,(k-1)/4, (k+1)/4.
 * https://oeis.org/A057526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; For n>1: if n is even, a(n)=1+a(n/2); else if n≡1 (mod
 *            4), a(n)=1+a((n-1)/4); else (n≡3 (mod 4)), a(n)=1+a((n+1)/4)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t steps = 0;
    while (n > 1)
    {
        if ((n & 1) == 0)                /* n even: use n/2 */
        {
            n >>= 1;
        }
        else if ((n & 3) == 1)           /* n≡1 (mod 4): use (n‑1)/4 */
        {
            n >>= 2;                     /* (n-1)/4 == n>>2 when n≡1 mod 4 */
        }
        else                             /* n≡3 (mod 4): use (n+1)/4 */
        {
            /* n+1 may overflow 64‑bit, do it with 128‑bit arithmetic if needed */
            unsigned __int128 tmp = (unsigned __int128)n + 1;
            n = (idx_t)(tmp >> 2);
        }
        ++steps;
    }
    return steps;
}
