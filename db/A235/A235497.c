/*
 * A235497  2n concatenated with n.
 * https://oeis.org/A235497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*n*10^floor(log_10(n)+1) + n
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenates 2*n and n.  Computes the number of decimal
   digits of n by a simple loop and builds 10^digits.  */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for offset 1, but return 0 for safety */
        return 0;
    seq_t temp = (seq_t)n;
    int d = 0;
    do {
        temp /= 10;
        d++;
    } while (temp);
    seq_t pow = 1;
    for (int i = 0; i < d; i++)
        pow *= 10;
    seq_t two_n = ((seq_t)n) * 2;
    return two_n * pow + (seq_t)n;
}
