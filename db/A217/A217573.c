/*
 * A217573  Number of integers between -(2*n+1)*Pi and (2*n+2)*Pi.
 * https://oeis.org/A217573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2*n+1)*π) + floor((2*n+2)*π) + 1
 * Range:     u8 none, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The rational 355/113 approximates π with an error of < 2.67e‑7.  */
/*  For all n ≤ 10000 (k ≤ 20002) we have k*|355/113‑π| < 1,    */
/*  so floor(k*355/113) = floor(k*π).  This approach uses only  */
/*  integer arithmetic and works for any seq_t.                 */
seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;                /* promote index to seq_t          */
    seq_t k1 = 2 * ns + 1;              /* 2n+1                             */
    seq_t k2 = 2 * ns + 2;              /* 2n+2                             */
    seq_t a1 = (k1 * (seq_t)355) / 113; /* floor((2n+1)*π) approximated     */
    seq_t a2 = (k2 * (seq_t)355) / 113; /* floor((2n+2)*π) approximated     */
    return a1 + a2 + 1;                 /* +1 as per the definition        */
}
