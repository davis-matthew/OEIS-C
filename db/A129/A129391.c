/*
 * A129391  Expansion of phi(-x) * phi(x^5) / (chi(-x^2) * chi(-x^10)) in
 *          powers of x where phi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A129391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = b(2*n+1), where b is the multiplicative function
 *
 *   b(5^e)             = 1
 *   b(p^e) = (-1)^e*(e+1)     if p ≡ 3 or 7 (mod 20)
 *   b(p^e) = (e+1)            if p ≡ 1 or 9 (mod 20)
 *   b(p^e) = (1+(-1)^e)/2     if p ≡ 11,13,17 or 19 (mod 20)
 *   b(2^e) = 0                (never occurs for 2*n+1)
 *
 * The function implements this definition via prime factorisation of
 * 2*n+1.  For small values (n ≤ 10 000) trial division is sufficient.
 * It works for any n that fits in the idx_t type.
 */

#include <stdint.h>   /* for uint64_t */

static seq_t compute_b(uint64_t N)
{
    if (N == 1) return 1;          /* b(1) = 1 */

    seq_t result = 1;

    /* factor 3,5,7,... */
    for (uint64_t p = 3; p <= N / p; p += 2) {
        if (N % p != 0) continue;
        uint64_t exp = 0;
        while (N % p == 0) {
            N /= p;
            ++exp;
        }

        seq_t contrib;
        if (p == 5) {
            contrib = 1;                      /* b(5^e) = 1 */
        } else {
            uint64_t mod = p % 20;
            if (mod == 3 || mod == 7) {
                contrib = (seq_t)(exp + 1);
                if (exp & 1) contrib = -contrib;
            } else if (mod == 1 || mod == 9) {
                contrib = (seq_t)(exp + 1);
            } else { /* mod == 11,13,17,19 */
                contrib = (exp & 1) ? 0 : 1;
            }
        }
        result *= contrib;
    }

    /* Remaining prime factor, if any */
    if (N > 1) {
        /* N is prime, exponent 1 */
        uint64_t exp = 1;
        seq_t contrib;
        uint64_t mod = N % 20;
        if (N == 5) {
            contrib = 1;
        } else if (mod == 3 || mod == 7) {
            contrib = (seq_t)(exp + 1);
            if (exp & 1) contrib = -contrib;
        } else if (mod == 1 || mod == 9) {
            contrib = (seq_t)(exp + 1);
        } else { /* mod == 11,13,17,19 */
            contrib = 0;                     /* (1+(-1)^1)/2 = 0 */
        }
        result *= contrib;
    }

    return result;
}

seq_t a(idx_t n)
{
    /* Compute a(n) = b(2*n + 1) */
    uint64_t m = 2 * n + 1;
    return compute_b(m);
}
