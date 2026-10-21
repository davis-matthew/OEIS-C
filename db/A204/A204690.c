/*
 * A204690  n^n (mod 5).
 * https://oeis.org/A204690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n mod 5
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0^0 is defined as 1 in this sequence */
    if (n == 0) return (seq_t)1;

    /* If n is a multiple of 5, the result is 0 (except n==0) */
    if ((n % 5) == 0) return (seq_t)0;

    /* Compute n^n mod 5 using fast modular exponentiation. */
    seq_t base   = (seq_t)(n % 5);
    seq_t result = (seq_t)1;
    idx_t exp    = n;

    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % 5;
        }
        base = (base * base) % 5;
        exp >>= 1;
    }

    return result;
}
