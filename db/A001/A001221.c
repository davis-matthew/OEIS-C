/*
 * A001221  Number of distinct primes dividing n (also called omega(n)).
 * https://oeis.org/A001221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, core, changed
 * Formula:   a(n) = number of distinct prime divisors of n (omega function)
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 127 terms through n = 9986 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    
    seq_t count = 0;
    idx_t temp = n;
    
    /* Check for factor 2 */
    if (temp % 2 == 0) {
        count++;
        while (temp % 2 == 0)
            temp /= 2;
    }
    
    /* Check for odd factors from 3 onwards */
    for (idx_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            count++;
            while (temp % d == 0)
                temp /= d;
        }
    }
    
    /* If temp > 1, then it's a prime factor */
    if (temp > 1)
        count++;
    
    return count;
}
