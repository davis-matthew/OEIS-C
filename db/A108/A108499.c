/*
 * A108499  Number of values of k (1<=k<=n) where k^(n+1) = k mod n, or
 *          equivalently where sum_i{1<=i<=n} k^i = 0 mod n.
 * https://oeis.org/A108499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ 1 ≤ k ≤ n | k^(n+1) ≡ k (mod n) }
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of k in [1,n] satisfying k^(n+1) ≡ k (mod n).
   Straightforward O(n log n) algorithm using modular exponentiation
   with 128‑bit multiplication to stay safe for all supported
   integer widths.  It works for all n ≥ 1, includes the case
   k = n (mod n = 0), and returns the correct value in seq_t. */

#include <stdint.h>
#include <stdbool.h>

/* Generic 64‑bit modular multiplication using 128‑bit product. */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)(((unsigned __int128)a * b) % mod);
}

/* Generic modular exponentiation (base^exp mod mod). */
static inline uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    uint64_t pow = base % mod;
    while (exp) {
        if (exp & 1U) result = mul_mod64(result, pow, mod);
        pow = mul_mod64(pow, pow, mod);
        exp >>= 1U;
    }
    return result;
}

/* public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* safety, though offset starts at 1 */

    uint64_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* compute k^(n+1) mod n */
        uint64_t mod = (uint64_t)n;
        uint64_t pow = pow_mod64((uint64_t)k, n + 1, mod);
        uint64_t rhs = (uint64_t)k % mod;  /* k mod n (0 for k==n) */
        if (pow == rhs) ++count;
    }
    return (seq_t)count;
}
