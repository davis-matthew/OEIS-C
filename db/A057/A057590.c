/*
 * A057590  Numbers of the form k*(k^3 +- 1)/2.
 * https://oeis.org/A057590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 38, u32 n <= 606, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(2k-1) = k*(k³-1)/2, a(2k) = k*(k³+1)/2, a(1)=0 */

seq_t a(idx_t n)
{
    /* Handle the first index explicitly (optional, for clarity) */
    if (n < 1)
        return 0;

    /* Determine whether n is odd or even */
    if (n & 1)                /* odd: use the "-1" formula */
    {
        idx_t k = (n + 1) >> 1;        /* k = (n+1)/2 */
        seq_t ks  = (seq_t)k;          /* promote to seq_t */
        seq_t ks2 = ks * ks;           /* k² */
        seq_t ks3 = ks2 * ks;          /* k³ */
        return ks * (ks3 - 1) / 2;     /* k*(k³-1)/2 */
    }
    else                       /* even: use the "+1" formula */
    {
        idx_t k = n >> 1;              /* k = n/2 */
        seq_t ks  = (seq_t)k;
        seq_t ks2 = ks * ks;
        seq_t ks3 = ks2 * ks;
        return ks * (ks3 + 1) / 2;     /* k*(k³+1)/2 */
    }
}
