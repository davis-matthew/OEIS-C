/*
 * A077309  Concatenation of n numbers starting with n.
 * https://oeis.org/A077309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenate the n consecutive integers starting at n
 *            (i.e., a(n)=concat_{k=0}^{n-1} (n+k))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 6, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed powers of 10 up to 10^20 (sufficient for 64‑bit numbers) */
    static seq_t pow10[21];
    static int initialized = 0;
    if (!initialized) {
        pow10[0] = (seq_t)1;
        for (int i = 1; i < 21; ++i)
            pow10[i] = pow10[i-1] * (seq_t)10;
        initialized = 1;
    }

    seq_t result = (seq_t)0;
    uint64_t start = (uint64_t)n;   /* n fits in uint64_t by definition of idx_t */

    for (idx_t i = (idx_t)0; i < n; ++i) {
        uint64_t val = start + (uint64_t)i;
        /* Count decimal digits of val (val >= 1) */
        uint64_t x = val;
        int d = 0;
        do { ++d; x /= 10; } while (x);

        result = result * pow10[d] + (seq_t)val;
    }

    return result;
}
