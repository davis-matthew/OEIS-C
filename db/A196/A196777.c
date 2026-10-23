/*
 * A196777  Sum (mod n) of the distinct residues of x^n (mod n), x=0..n-1.
 * https://oeis.org/A196777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct residues of x^n (mod n), x = 0..n-1 (mod
 *            n)
 * Range:     u8 n <= 33, u16 n <= 2477, u32 n <= 9255, u64 n <= 9255,
 *            u128 n <= 9255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* modular exponentiation: base^exp mod mod */
static seq_t modpow(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t res = (seq_t)1;
    if (mod == (seq_t)0) return (seq_t)0;                 /* n = 0 never occurs */
    res %= mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1ULL) {
            res = (res * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;          /* a(1) = 0 */

    /* Fast useful shortcuts: odd n → 0, power‑of‑two n → 1 */
    if ((n & 1ULL) == 1ULL) return (seq_t)0;                     /* odd n */
    if ((n & (n - 1ULL)) == 0ULL) return (seq_t)1;              /* 2^m */

    size_t sz = (size_t)n;
    uint8_t *seen = (uint8_t *)calloc(sz, sizeof(uint8_t));     /* C++ cast */
    if (!seen) return (seq_t)0;                                  /* allocation failure */

    seq_t sum = (seq_t)0;
    for (idx_t x = 0; x < n; ++x) {
        seq_t r = modpow((seq_t)x, (uint64_t)n, (seq_t)n);
        idx_t idx = (idx_t)r;          /* r < n because of modulus */
        if (!seen[idx]) {
            seen[idx] = 1;
            sum += r;
        }
    }
    free(seen);
    return sum % (seq_t)n;
}
