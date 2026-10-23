/*
 * A109451  a(1)=1; a(n) = smallest positive integer not already present such
 *          that a(n-1) and a(n) have a different number of 1's in their
 *          binary expansions.
 * https://oeis.org/A109451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 1 if n == 2 or 4 (mod 8), * a(n) = n - 1 if n == 3 or
 *            5 (mod 8), * otherwise a(n) = n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long mod = n & 7ULL;
    seq_t  val = (seq_t)n;
    if (mod == 2ULL || mod == 4ULL)
        return val + 1;
    if (mod == 3ULL || mod == 5ULL)
        return val - 1;
    return val;
}
