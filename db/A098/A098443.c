/*
 * A098443  Expansion of 1/sqrt(1-8*x-4*x^2).
 * https://oeis.org/A098443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence):  a(0) = 1,  a(1) = 4
 *  n * a(n) = 4*(2*n-1)*a(n-1) + 4*(n-1)*a(n-2)   for n ≥ 2
 *  (Thus a(n) = (4*(2*n-1)*a(n-1) + 4*(n-1)*a(n-2)) / n) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)4;   /* a(n-1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t curr;
        if (sizeof(seq_t) <= 8)          /* Built‑in integer type: use 128‑bit for safety */
        {
            unsigned __int128 mul1 = (unsigned __int128)prev1 * (unsigned __int128)(4 * (2 * i - 1));
            unsigned __int128 mul2 = (unsigned __int128)prev2 * (unsigned __int128)(4 * (i - 1));
            unsigned __int128 sum  = mul1 + mul2;
            curr = (seq_t)(sum / (unsigned __int128)i);
        }
        else                             /* Arbitrary‑precision or >64‑bit type */
        {
            seq_t mul1 = prev1 * (seq_t)(4 * (2 * i - 1));
            seq_t mul2 = prev2 * (seq_t)(4 * (i - 1));
            seq_t sum  = mul1 + mul2;
            curr = sum / (seq_t)i;
        }
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
