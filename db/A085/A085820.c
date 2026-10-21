/*
 * A085820  Possible two-digit endings of primes (with leading zeros).
 * https://oeis.org/A085820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  full, nonn, fini, base
 * Formula:   a(n) = the nth element of the sorted list of all integers e with
 *            * 0 ≤ e < 100 and gcd(e,100) = 1. This is equivalent to e being
 *            odd and not * divisible by 5. The sequence is finite (40 terms)
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n should be ≥ 1 for defined terms; return 0 for out‑of‑range. */
    if (n == 0)
        return (seq_t)0;

    idx_t count = 0;
    for (idx_t e = 0; e < 100; ++e) {
        /* Test gcd(e,100) = 1 by checking divisibility by 2 and 5. */
        if ((e % 2) && (e % 5)) {
            ++count;
            if (count == n)
                return (seq_t)e;
        }
    }
    /* If n exceeds the 40 valid values, return 0. */
    return (seq_t)0;
}
