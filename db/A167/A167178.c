/*
 * A167178  a(n)^3 ends in n^3.
 * https://oeis.org/A167178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = least k>n such that decimal expansion of k^3 ends in n^3
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   unsigned_digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/unsigned_digit_count.h"

seq_t a(idx_t n)
{
    /* Convert idx_t to a form we can work with comfortably. */
    uint64_t n64 = (uint64_t)n;

    /* Compute n^3, using 128‑bit arithmetic to avoid overflow. */
    unsigned __int128 n3 = (unsigned __int128)n64 * n64 * n64;

    /* Determine number of digits of n^3. */
    unsigned d = unsigned_digit_count((uint64_t)n3);

    /* Compute modulus M = 10^d (fits in 64 bit for the intended range). */
    uint64_t M = 1;
    for (unsigned i = 0; i < d; ++i)
        M *= 10ULL;

    /* Compute n^3 modulo M.  For our range n^3 < M in many cases, but we
       handle the general case with modulo. */
    uint64_t cube_n_mod;
    if (n3 < M)
        cube_n_mod = (uint64_t)n3;
    else
        cube_n_mod = (uint64_t)(n3 % M);

    /* If n is coprime to 10, the trivial solution k = n + M always works. */
    if ((n64 % 2ULL != 0) && (n64 % 5ULL != 0)) {
        return (seq_t)(n64 + M);
    }

    /* For n sharing factors with 10, search linearly for the first k > n
       that satisfies k^3 ≡ n^3 (mod M).  The distance to the next solution
       is observed to be small for the range of n considered (up to ~1e4). */
    for (uint64_t k = n64 + 1; ; ++k) {
        uint64_t k_mod = k % M;                      /* reduce to [0,M) */
        unsigned __int128 t = (unsigned __int128)k_mod * k_mod % M;
        t = t * k_mod % M;                           /* k^3 mod M */
        if ((uint64_t)t == cube_n_mod) {
            return (seq_t)k;
        }
    }
}
