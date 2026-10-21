/*
 * A053601  Number of bases of an n-dimensional vector space over GF(2).
 * https://oeis.org/A053601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n-1)(2^n-2)...(2^n-2^(n-1))/n! (number of bases of
 *            GF(2)^n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(0)=1, a(1)=1 */

    seq_t result = 1;
    /* compute 2^n */
    seq_t two_pow_n = 1;
    for (idx_t i = 0; i < n; ++i)
        two_pow_n *= (seq_t)2;             /* built‑in overflow is fine for unsigned types */

    seq_t power2k = 1;                      /* 2^k, starts with k=0 */
    for (idx_t k = 0; k < n; ++k) {
        seq_t term = two_pow_n - power2k;   /* 2^n - 2^k */
        result *= term;                    /* multiply the term */
        result /= (seq_t)(k + 1);          /* divide by k+1 (factorial part) */
        power2k *= (seq_t)2;               /* next power of two */
    }

    return result;
}
