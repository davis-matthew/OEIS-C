/*
 * A094735  Number of connected 3-element multiantichains on a labeled n-set.
 * https://oeis.org/A094735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7^n - 6*5^n + 20*3^n + 3*4^n - 39*2^n + 35) / 6 for n>0,
 *            a(0) = 0. The computation uses 128‑bit intermediates for the
 *            64‑bit build so that intermediate values do not overflow
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __SIZEOF_INT128__
typedef unsigned __int128 u128;
#endif

/* Generic fast‑exponentiation that works for any seq_t type.  For
   small types this function will not overflow because we use a specialised
   path below. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

extern seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

#ifdef __SIZEOF_INT128__
    /* When seq_t is 8 bytes or fewer we use u128 to avoid overflow
       during the intermediate products.  For larger seq_t types
       (arbitrary‑precision) we fall back to the generic pow_seq
       routine. */
    if (sizeof(seq_t) <= 8) {
        u128 p7 = 1, p5 = 1, p4 = 1, p3 = 1, p2 = 1;
        for (idx_t i = 0; i < n; ++i) {
            p7 *= 7;
            p5 *= 5;
            p4 *= 4;
            p3 *= 3;
            p2 *= 2;
        }
        u128 num =  p7
                  - (u128)6 * p5
                  + (u128)20 * p3
                  + (u128)3  * p4
                  - (u128)39 * p2
                  + 35;
        u128 res = num / 6;
        return (seq_t)res;   /* fits in seq_t because a(n) fits */
    }
#endif

    /* Fallback for big‑integer seq_t types.  Intermediate values may
       be very large, but the final division by 6 is exact. */
    seq_t p7 = pow_seq((seq_t)7, n);
    seq_t p5 = pow_seq((seq_t)5, n);
    seq_t p4 = pow_seq((seq_t)4, n);
    seq_t p3 = pow_seq((seq_t)3, n);
    seq_t p2 = pow_seq((seq_t)2, n);

    seq_t num =   p7
                - (seq_t)6 * p5
                + (seq_t)20 * p3
                + (seq_t)3 * p4
                - (seq_t)39 * p2
                + 35;
    return num / (seq_t)6;
}
