/*
 * A175676  a(n) = binomial(n,3) mod n.
 * https://oeis.org/A175676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,3) mod n = n/3 if n%3==0 else 0
 * Range:     u8 n <= 767, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The binomial coefficient binomial(n,3) is n*(n-1)*(n-2)/6.
       Since it contains a factor n, dividing by n leaves
       (n-1)*(n-2)/6.  For n divisible by 3 this is an integer
       and equals n/3; otherwise the result is 0. */
    if (n % 3 == 0) {
        return (seq_t)(n / 3);
    }
    return 0;
}
