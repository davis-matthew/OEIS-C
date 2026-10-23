/*
 * A177712  Even numbers divisible by an odd prime.
 * https://oeis.org/A177712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (n-th positive integer that is not a power of 2)
 *            (i.e. 2*A057716(n) – even numbers that have an odd prime
 *            factor). The value is computed by simple linear search, skipping
 *            powers of two
 * Range:     u8 n <= 120, u16 n <= 9620, u32 n <= 9620, u64 n <= 9620,
 *            u128 n <= 9620, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work with seq_t values: cur holds the current integer being tested. */
    seq_t cur = 3;            /* first candidate > 2 (since 2 is a power of 2) */
    idx_t  count = 0;         /* number of accepted integers found so far */

    while (1) {
        /* Skip powers of two: n & (n-1) == 0  <=> n is a power of two */
        if ((cur & (cur - 1)) != 0) {
            count++;
            if (count == n)
                return cur * 2;   /* a(n) is twice that integer */
        }
        cur++;                 /* move to next integer */
    }
}
