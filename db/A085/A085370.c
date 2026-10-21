/*
 * A085370  Niven (or Harshad) numbers that are not divisible by 3.
 * https://oeis.org/A085370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 2501, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A085370: Niven (Harshad) numbers that are not divisible by 3.
    a(n) is the n-th positive integer such that a(n) % digit_sum(a(n)) == 0
    and digit_sum(a(n)) % 3 != 0.  The sequence is closed under
    multiplication by 10.  The following implementation enumerates
    natural numbers until the n-th such integer is found.  It runs in
    O(n) time and uses only integer arithmetic. */
seq_t a(idx_t n)
{
    if (n == 0)                /* not defined for 0, return 0 */
        return (seq_t)0;
    seq_t x = 1;               /* candidate number */
    idx_t  found = 0;          /* how many terms have been found */

    while (found < n) {
        /* compute the sum of the decimal digits of x */
        seq_t d = 0;
        seq_t t = x;
        while (t > 0) {
            d += t % 10;
            t /= 10;
        }

        /* test Harshad condition and exclusion of divisibility by 3 */
        if (d > 0 && x % d == 0 && d % 3 != 0) {
            found++;
            if (found == n)
                return x;
        }
        x++;
    }
    /* unreachable code, but satisfy the compiler */
    return x;
}
