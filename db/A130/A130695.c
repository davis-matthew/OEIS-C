/*
 * A130695  Number of ways to write n as (a+1)(b+1)(c+1) - abc with a, b, c
 *          nonnegative integers.
 * https://oeis.org/A130695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(a,b,c)∈ℕ₀³ : (a+1)(b+1)(c+1)-abc = n}|
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset is 1, but guard against 0 */
    uint64_t N = (uint64_t)n;            /* use 64‑bit arithmetic for safety */
    seq_t count = 0;

    /* Iterate over a and b, then compute c from the equation */
    for (idx_t a = 0; (uint64_t)(a + 1) <= N; ++a) {
        uint64_t a1 = (uint64_t)(a + 1);
        uint64_t max_b1 = N / a1;          /* (b+1) cannot exceed this */
        for (idx_t b = 0; (uint64_t)(b + 1) <= max_b1; ++b) {
            uint64_t b1 = (uint64_t)(b + 1);
            uint64_t prod = a1 * b1;
            uint64_t numerator = N - prod;     /* n - (a+1)(b+1)  */
            uint64_t s = (uint64_t)(a + b + 1); /* a + b + 1   */
            if (numerator % s == 0) {
                /* c = numerator / s is automatically >= 0 */
                ++count;
            }
        }
    }
    return count;
}
