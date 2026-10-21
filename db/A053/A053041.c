/*
 * A053041  Smallest n-digit number divisible by n.
 * https://oeis.org/A053041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, nice
 * Formula:   a(n) = n*ceiling(10^(n-1)/n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute r = 10^(n-1) mod n */
    if (n == 0) return 0;          /* safety, though n >= 1 per spec */
    uint64_t mod = (uint64_t)n;
    uint64_t res = 1 % mod;        /* 10^0 = 1 */
    uint64_t base = 10 % mod;
    uint64_t exp = (uint64_t)(n - 1);

    while (exp > 0) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    /* diff = (n - r) mod n          */
    idx_t diff = (idx_t)mod;        /* mod == n */
    diff = (diff - (idx_t)res) % n;

    /* Build 10^(n-1) as a seq_t */
    seq_t value = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        value = value * (seq_t)10;
    }

    /* Add the minimal adjustment to reach a multiple of n */
    value = value + (seq_t)diff;

    return value;
}
