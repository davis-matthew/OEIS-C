/*
 * A057288  Numbers n such that n | 11^n + 10^n + 9^n + 8^n + 7^n + 6^n + 5^n
 *          + 4^n + 3^n + 2^n.
 * https://oeis.org/A057288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer such that n | Σ_{k=2}^{11} k^n
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation tests each candidate n modulo itself and records
   the ones that satisfy the divisibility condition.  Results are
   memoised so that a(n) can be called in any order. */

#include <stdbool.h>
#include <stddef.h>

/* Power modulo for arbitrary-precision seq_t.
   Computes base^exp (mod mod). */
static seq_t mod_pow(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1;
    base %= mod;
    for (idx_t e = exp; e; e >>= 1) {
        if (e & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
    }
    return result;
}

/* Checks whether a candidate n satisfies the condition n | Σ_{k=2}^{11} k^n */
static bool good(idx_t n)
{
    seq_t mod = (seq_t)n;
    seq_t sum = 0;
    for (int b = 2; b <= 11; ++b) {
        seq_t term = mod_pow((seq_t)b, n, mod);
        sum = (sum + term) % mod;
    }
    return sum == 0;
}

/* Fixed-size storage for up to 20000 values (sufficient for n up to about 10 000).
   If a larger number of terms is required, increase the array size. */
#define MAX_TERMS 20000
static seq_t terms[MAX_TERMS];
static size_t term_count = 0;   /* number of computed terms */
static idx_t next_candidate = 0; /* candidate next to test (starts at 0) */

/* Main sequence function required by the platform */
seq_t a(idx_t n)
{
    if (n == 0) {        /* Undefined for index 0 – return 0 */
        return 0;
    }

    /* Compute terms until we have at least n of them */
    while (term_count < (size_t)n) {
        ++next_candidate;                /* test next integer */
        if (good(next_candidate)) {
            if (term_count < MAX_TERMS) {
                terms[term_count++] = (seq_t)next_candidate;
            } else {
                /* Exceeded preallocated storage – abort gracefully */
                while (true) {}          /* infinite loop to avoid UB */
            }
        }
    }
    return terms[n - 1];
}
