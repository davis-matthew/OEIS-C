/*
 * A147543  a(n) = (8*5^n + 5*3^(n+1) - 5*2^n)/3.
 * https://oeis.org/A147543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*5^n + 5*3^(n+1) - 5*2^n)/3 (Recurrence: a(0)=6,
 *            a(1)=25, a(2)=105, a(n)=10*a(n-1)-31*a(n-2)+30*a(n-3))
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow5 = 1;     /* 5^i */
    seq_t pow3 = 3;     /* 3^(i+1) */
    seq_t pow2 = 1;     /* 2^i */
    seq_t result = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t num = 8 * pow5 + 5 * pow3 - 5 * pow2;
        result = num / 3;            /* exact division by 3 */

        if (i == n)
            return result;           /* return the requested term */

        /* advance powers for the next i */
        pow5 *= 5;
        pow3 *= 3;
        pow2 *= 2;
    }

    return result; /* fallback (should never be reached) */
}
