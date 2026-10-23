/*
 * A181615  Triangle read by rows: T(n,k) = 2^(n-1) mod semiprime(k).
 * https://oeis.org/A181615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(row−1) mod semiprime(col) where (row, col) are the
 *            1‑based coordinates of the index n in the triangular array (row
 *            sums are 1, 3, 6, 10, …)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   ensure_semiprimes, semiprime_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_semiprimes.h"
#include "../utils/semiprime_at.h"

static seq_t powmod_uint(uint64_t base, idx_t exp, uint32_t mod)
{
    /* Computes base^exp mod mod for small mod (≤ 2^32). */
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    /* Find the row r and column k from the triangular indexing. */
    idx_t r = 1;
    idx_t cumulative = 1;
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }
    idx_t prev = cumulative - r;
    idx_t k = n - prev; /* 1‑based column */

    /* Compute 2^(r-1) mod semiprime(k). */
    uint32_t mod = semiprime_at((size_t)k);
    return powmod_uint(2, r - 1, mod);
}
