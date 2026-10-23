/*
 * A132385  Number of distinct primes among the cubes mod n.
 * https://oeis.org/A132385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = |{p prime | p ≡ k³ (mod n) for some integer k}|
 * Range:     u8 n <= 1631, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count distinct primes appearing among the cubes modulo n.
 * Complexity: O(n √n) – fast enough for n ≤ 10⁴. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Small trial‑division primality test for 64‑bit integers. */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition – the loop handles it automatically. */
    if (n == 0) return 0;            /* safety for accidental n == 0 */

    /* Allocate a Boolean array to remember whether each prime residue
     * was already seen.  All values < n, so the index fits in size_t. */
    size_t Ns = (size_t)n;
    bool *seen = (bool *)calloc(Ns, sizeof(bool));
    if (!seen) return 0;             /* allocation failure → return 0 */

    uint64_t N = (uint64_t)n;        /* modulus */
    seq_t count = 0;                 /* result accumulator */

    for (idx_t k = 0; k < n; ++k) {
        uint64_t kk = (uint64_t)k;
        /* Compute k³ mod n safely without overflow for n ≤ 10⁴. */
        uint64_t cube_mod = ((kk * kk) % N * kk) % N;
        uint64_t val = cube_mod;    /* the residue */

        if (val < 2) continue;      /* 0 and 1 are not prime */

        if (!is_prime_uint64(val))
            continue;               /* not a prime residue */

        size_t idx = (size_t)val;   /* val < n */
        if (!seen[idx]) {
            seen[idx] = true;
            ++count;                /* distinct prime found */
        }
    }

    free(seen);
    return count;
}
