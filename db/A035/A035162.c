/*
 * A035162  Number of positive odd solutions to equation x^2 + 7y^2 = 8n.
 * https://oeis.org/A035162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A035162: Number of positive odd solutions to x^2 + 7y^2 = 8n.
 * Formula (multiplicative): 
 *   a(2^e) = a(7^e) = 1
 *   For odd prime p:
 *     if p ≡ 1,2,4 (mod 7) → a(p^e) = e + 1
 *     if p ≡ 3,5,6 (mod 7) → a(p^e) = (1 + (–1)^e)/2   (0 if e odd, 1 if e even)
 *   a is multiplicative: a(m·n) = a(m)·a(n) for coprime m,n.
 *
 * The function below computes a(n) for any n ≥ 1 by factoring n and
 * applying the above rules.  It is efficient for n up to at least 10 000
 * and scales reasonably for larger values using trial division.
 */

#include <stdint.h>  /* for uint64_t if needed (idx_t and seq_t are defined by the build) */

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined by OEIS, but safe fallback */

    seq_t result = 1;
    idx_t temp = n;

    /* trial division ---------------------------------------------- */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* apply multiplicative rules */
            if (p == 2 || p == 7) {
                /* value stays 1, no change to result */
            } else {
                idx_t mod = p % 7;
                if (mod == 1 || mod == 2 || mod == 4) {
                    result *= (seq_t)(e + 1);
                } else if (mod == 3 || mod == 5 || mod == 6) {
                    /* odd exponent gives factor 0 → entire product is 0 */
                    if (e & 1) return 0;
                    /* even exponent contributes factor 1 (no effect) */
                }
            }
        }
    }

    /* any remaining prime factor > sqrt(original n) ----------------- */
    if (temp > 1) {          /* temp is now a prime (exponent 1)  */
        idx_t p = temp;
        if (p == 2 || p == 7) {
            /* contributes 1 */
        } else {
            idx_t mod = p % 7;
            if (mod == 1 || mod == 2 || mod == 4) {
                result *= 2;   /* e+1, where e = 1 */
            } else {          /* mod 3,5,6 with e=1 (odd) → 0 product */
                return 0;
            }
        }
    }

    return result;
}
