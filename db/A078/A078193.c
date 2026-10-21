/*
 * A078193  In the following triangle the n-th row contains n n-digit (or
 *          (n-1)-digit) numbers whose concatenation (with a 0 prefixed for
 *          (n-1)-digit numbers) gives a substring of the cyclic concatenation
 *          of 1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,...: 1; 12 34; 123
 *          456 789; 1234 5678 9012 3456; 12345 67...
 * https://oeis.org/A078193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = integer formed by n consecutive digits of the cyclic *
 *            sequence 1,2,3,4,5,6,7,8,9,0,1,... starting with leading digit *
 *            ((n-1)*n+1) mod 10. Each subsequent digit increases by 1 modulo
 *            10
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t leading = ((n - 1) * n + 1) % 10;
    unsigned int s = (unsigned int)leading;
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i) {
        unsigned int d = (s + (unsigned int)(i % 10)) % 10;
        val = val * 10 + d;
    }
    return val;
}
