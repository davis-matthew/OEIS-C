/*
 * A057522  Start with 73, to get the next term divide by the smallest prime
 *          factor < 13 if it exists, else multiply by 13 and add 1.
 * https://oeis.org/A057522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=73; for n>0, a(n)= a(n-1) divided by the smallest prime
 *            factor of a(n-1) that is <13 (2,3,5,7,11), if such a factor
 *            exists; otherwise a(n)=13*a(n-1)+1
 * Range:     u8 n <= 0, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the next value in the sequence given a current term. */
static inline seq_t next_term(seq_t x)
{
    /* Test divisibility by the primes <13 in increasing order. */
    if (x % 2u == 0) return x / 2u;
    if (x % 3u == 0) return x / 3u;
    if (x % 5u == 0) return x / 5u;
    if (x % 7u == 0) return x / 7u;
    if (x % 11u == 0) return x / 11u;
    /* No small prime factor: apply the 13×+1 rule. */
    return 13u * x + 1u;
}

seq_t a(idx_t n)
{
    /* a(0) = 73; compute iteratively for 0 < i ≤ n. */
    seq_t val = 73u;
    /* If n is zero, return the starting value. */
    if (n == 0) return val;
    for (idx_t i = 0; i < n; ++i)
    {
        val = next_term(val);
    }
    return val;
}
