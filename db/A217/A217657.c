/*
 * A217657  Delete the initial digit in decimal representation of n.
 * https://oeis.org/A217657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n mod 10^floor(log10(n))
 * Range:     u8 n <= 1255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For single-digit n, the result is defined to be 0 */
    if (n <= 9) return 0;

    /* Compute the largest power of 10 that does not exceed n/10.
       This is 10^(digits(n)-1). */
    idx_t pow10 = 1;
    while (pow10 <= n / 10) {
        pow10 *= 10;
    }

    /* The removal of the leading digit is just n modulo this power */
    idx_t mod = n % pow10;
    return (seq_t)mod;
}
