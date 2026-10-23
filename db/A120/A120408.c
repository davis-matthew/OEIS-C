/*
 * A120408  a(n) = n^1*(n+1)^2*(n+2)^3*(n+3)^4*(n+4)^5/(1!*2!*3!*4!*5!).
 * https://oeis.org/A120408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^1*(n+1)^2*(n+2)^3*(n+3)^4*(n+4)^5/(1!*2!*3!*4!*5!)
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominator equals 34560 = 1!*2!*3!*4!*5! */

seq_t a(idx_t n)
{
    seq_t res = 1;
    uint64_t denom = 34560;   /* prime factors 2^8 * 3^2 * 5 */

    /* n^1  */
    {
        idx_t base = n;
        int exp = 1;
        for (int i = 0; i < exp; ++i) {
            seq_t factor = (seq_t)base;
            while ((denom & 1ULL) && (factor % 2ULL == 0)) { denom >>= 1; factor >>= 1; }
            while ((denom % 3ULL == 0) && (factor % 3ULL == 0)) { denom /= 3; factor /= 3; }
            while ((denom % 5ULL == 0) && (factor % 5ULL == 0)) { denom /= 5; factor /= 5; }
            res *= factor;
        }
    }

    /* (n+1)^2  */
    {
        idx_t base = n + 1;
        int exp = 2;
        for (int i = 0; i < exp; ++i) {
            seq_t factor = (seq_t)base;
            while ((denom & 1ULL) && (factor % 2ULL == 0)) { denom >>= 1; factor >>= 1; }
            while ((denom % 3ULL == 0) && (factor % 3ULL == 0)) { denom /= 3; factor /= 3; }
            while ((denom % 5ULL == 0) && (factor % 5ULL == 0)) { denom /= 5; factor /= 5; }
            res *= factor;
        }
    }

    /* (n+2)^3  */
    {
        idx_t base = n + 2;
        int exp = 3;
        for (int i = 0; i < exp; ++i) {
            seq_t factor = (seq_t)base;
            while ((denom & 1ULL) && (factor % 2ULL == 0)) { denom >>= 1; factor >>= 1; }
            while ((denom % 3ULL == 0) && (factor % 3ULL == 0)) { denom /= 3; factor /= 3; }
            while ((denom % 5ULL == 0) && (factor % 5ULL == 0)) { denom /= 5; factor /= 5; }
            res *= factor;
        }
    }

    /* (n+3)^4  */
    {
        idx_t base = n + 3;
        int exp = 4;
        for (int i = 0; i < exp; ++i) {
            seq_t factor = (seq_t)base;
            while ((denom & 1ULL) && (factor % 2ULL == 0)) { denom >>= 1; factor >>= 1; }
            while ((denom % 3ULL == 0) && (factor % 3ULL == 0)) { denom /= 3; factor /= 3; }
            while ((denom % 5ULL == 0) && (factor % 5ULL == 0)) { denom /= 5; factor /= 5; }
            res *= factor;
        }
    }

    /* (n+4)^5  */
    {
        idx_t base = n + 4;
        int exp = 5;
        for (int i = 0; i < exp; ++i) {
            seq_t factor = (seq_t)base;
            while ((denom & 1ULL) && (factor % 2ULL == 0)) { denom >>= 1; factor >>= 1; }
            while ((denom % 3ULL == 0) && (factor % 3ULL == 0)) { denom /= 3; factor /= 3; }
            while ((denom % 5ULL == 0) && (factor % 5ULL == 0)) { denom /= 5; factor /= 5; }
            res *= factor;
        }
    }

    /* Any remaining denominator factors (should be 1) */
    if (denom != 1) {
        res /= (seq_t)denom;
    }

    return res;
}
