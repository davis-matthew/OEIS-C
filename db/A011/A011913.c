/*
 * A011913  a(n) = floor(n*(n - 1)*(n - 2)/31).
 * https://oeis.org/A011913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(n - 1)*(n - 2)/31)
 * Range:     u8 n <= 20, u16 n <= 127, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)          /* n = 0,1,2 -> 0 */
        return 0;

    /* For small seq_t types (≤64‑bit) use a 64‑bit intermediate
       to avoid overflow; otherwise use seq_t arithmetic directly. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned long long ni = (unsigned long long)n;
        unsigned long long prod = ni * (ni - 1);
        prod = prod * (ni - 2);
        return (seq_t)(prod / 31ULL);
    } else {
        seq_t x = (seq_t)n;
        seq_t prod = x * (x - 1);
        prod = prod * (x - 2);
        return prod / 31;
    }
}
