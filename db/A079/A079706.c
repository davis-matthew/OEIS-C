/*
 * A079706  Smallest positive exponent k such that (2n)^k+1 is prime, or -1 if
 *          no such k exists.
 * https://oeis.org/A079706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = smallest positive exponent k such that (2*n)^k + 1 is
 *            prime. All non‑zero terms are powers of two; a(n) = -1 if no
 *            such k exists
 * Range:     i8 n <= 3, i16 n <= 16, i32 n <= 18, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mod_pow.h"

/* Compute base^exp for seq_t, exp is idx_t. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp > 0) {
        if (exp & 1ULL) result = result * cur;
        exp >>= 1ULL;
        if (exp) cur = cur * cur;
    }
    return result;
}

/* Deterministic Miller–Rabin primality test for seq_t. */
static bool is_prime_seq(seq_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    const unsigned long long bases[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    size_t nb = sizeof(bases) / sizeof(bases[0]);

    for (size_t i = 0; i < nb; ++i) {
        unsigned long long aull = bases[i];
        if ((seq_t)aull >= n) continue;
        seq_t a = (seq_t)aull;
        seq_t x = mod_pow(a, (uint64_t)d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mod_pow(x, 2ULL, n);   /* x = x^2 mod n */
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return -1;            /* offset starts at 1 */
    seq_t base = (seq_t)2 * (seq_t)n;  /* base = 2*n */

    /* search k = 1,2,4,8,... up to 2^10 (=1024) */
    for (int t = 0; t <= 10; ++t) {
        seq_t k = (seq_t)1ULL << t;   /* k = 2^t */
        seq_t val = pow_seq_t(base, (idx_t)k) + 1;
        if (is_prime_seq(val)) return k;
    }
    return -1;
}
