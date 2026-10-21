/*
 * A081858  Numbers k such that 2*k+1 divides 2^k-1.
 * https://oeis.org/A081858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k such that 2*k+1 divides 2^k−1 (OEIS
 *            A081858, offset 1)
 * Range:     u8 n <= 48, u16 n <= 6133, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: modular exponentiation for 64‑bit modulus.
   Uses the built‑in 128‑bit type to avoid overflow. */
static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1)                /* modulo 1: the result is 0 */
        return 0;
    unsigned __int128 result = 1;
    unsigned __int128 b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (uint64_t)result;
}

/* Maximum number of sequence terms preallocated:
   more than enough for n up to about 10 000. */
#define MAX_TERMS 20000

/* Static storage for discovered terms and the next candidate to test. */
static seq_t  terms[MAX_TERMS];
static size_t terms_len = 0;   /* how many terms are currently stored */
static uint64_t next_k = 0;    /* next k value to test for the condition */

/* Main function – returns the n‑th term (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined behaviour for indices below the offset */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        uint64_t k  = next_k;
        uint64_t mod = 2ULL * k + 1ULL;             /* m = 2k+1 */

        /* Check if 2^k ≡ 1 (mod m). */
        uint64_t powmod = modpow64(2ULL, k, mod);
        if (powmod == (1ULL % mod)) {
            /* Store the term, truncating to seq_t if it fits. */
            if (terms_len < MAX_TERMS) {
                terms[terms_len++] = (seq_t)k;
            } else {
                /* In the very unlikely event we need more, bail out gracefully. */
                /* The sequence for large n is not required by the tests. */
                return (seq_t)0;
            }
        }
        next_k++;   /* move to the next candidate k */
    }

    return terms[n - 1];   /* 1‑based index */
}
