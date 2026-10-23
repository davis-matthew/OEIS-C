/*
 * A162761  Minimal total number of floors an elevator must move to transport
 *          n people initially waiting at floors i = 1, ..., n to their
 *          destination floors n-i+1 (= n, ..., 1), when the elevator can hold
 *          at most one person at a time and starts at floor 1, and no
 *          passenger may get off the elevator before rea...
 * https://oeis.org/A162761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n^2 + n + (-1)^n - 3)/2 for n > 1, a(1)=0
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1u) {
        return (seq_t)0;
    }
    unsigned long long sn = (unsigned long long)n;
    unsigned long long t = sn * sn + sn;
    if ((n & 1ULL) == 0) {
        t += 1ULL;
    } else {
        t -= 1ULL;
    }
    t -= 3ULL;
    return (seq_t)(t / 2ULL);
}
