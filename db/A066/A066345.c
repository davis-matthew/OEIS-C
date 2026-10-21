/*
 * A066345  Winning binary "same game" templates of length n as defined below.
 * https://oeis.org/A066345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n-1)= 2^(2*n-1) - n * 2^(n-1), a(2*n)= 2*a(2*n-1)-1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // undefined for n=0, return 0 as safeguard

    /* For odd indices: n = 2*k-1   (k = (n+1)/2) */
    if (n & 1U) {
        idx_t k = (n >> 1) + 1;    // (n+1)/2
        seq_t pow2_high = ((seq_t)1) << (unsigned long long)(2*k - 1);
        seq_t pow2_low  = ((seq_t)1) << (unsigned long long)(k - 1);
        return pow2_high - (seq_t)k * pow2_low;
    }

    /* For even indices: n = 2*k   (k = n/2) */
    {
        idx_t k = n >> 1;          // n/2
        seq_t pow2_high = ((seq_t)1) << (unsigned long long)(2*k - 1);
        seq_t pow2_low  = ((seq_t)1) << (unsigned long long)(k - 1);
        seq_t a_odd = pow2_high - (seq_t)k * pow2_low;
        return (a_odd << 1) - 1;    // 2*a_odd - 1
    }
}
