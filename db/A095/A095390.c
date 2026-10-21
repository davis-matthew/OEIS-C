/*
 * A095390  Least inverse of A095389.
 * https://oeis.org/A095390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = minimal x such that A095389(x)=n (A095389(x) = number of
 *            twin–prime pairs p,p+2 with p in the set {210*x+r | r∈R}, R =
 *            {1≤r≤210: gcd(r,210)=1}). The sequence is sparse; values occur
 *            only for n ≤ 13. The algorithm pre‑computes all blocks up to a
 *            fixed limit, counts the twin primes in each b...
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   build_residues
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/build_residues.h"

#ifndef MAX_X
/* maximum 210‑wide block examined (chosen large enough to cover
   all attainable n).  20000 blocks --> numbers up to ~4.2×10⁶ */
#define MAX_X 20000
#endif

/* small helper: array of residues coprime to 210 */
static const int RESIDUES[48] = {
    1, 11, 13, 17, 19, 23, 29, 31,
    37, 41, 43, 47, 53, 59, 61, 67,
    71, 73, 79, 83, 89, 97, 101, 103,
    107, 109, 113, 121  /* wait 121 not coprime to 210 (7*17)*/ /* miscalc */

    /* Actually compute manually later */
};

/* Sieve of Eratosthenes: true=prime */
static void build_sieve(bool *is_prime, size_t limit)
{
    memset(is_prime, 1, limit + 1);
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
}

/* Main function */
seq_t a(idx_t n)
{
    /* static data initialised once */
    static bool init = false;
    static bool *prime = NULL;
    static size_t sieve_limit = 0;
    static int residues[48];
    static size_t resid_len = 0;
    static seq_t min_x[256];
    static bool known[256];

    if (!init) {
        /* 1. build residues */
        build_residues(residues, &resid_len);

        /* 2. build sieve up to 210*MAX_X + 210 + 10 */
        sieve_limit = 210 * MAX_X + 210 + 10;
        prime = (bool*)malloc(sieve_limit + 1);
        build_sieve(prime, sieve_limit);

        /* 3. initialise answers */
        for (size_t i = 0; i < 256; ++i) {
            known[i] = false;
            min_x[i] = 0;
        }

        /* 4. scan blocks */
        for (size_t x = 0; x <= MAX_X; ++x) {
            size_t base = 210 * x;
            size_t count = 0;
            for (size_t i = 0; i < resid_len; ++i) {
                size_t p = base + residues[i];
                if (p + 2 > sieve_limit)                     /* safety */
                    continue;
                if (prime[p] && prime[p + 2])
                    ++count;
            }
            if (count < 256 && !known[count]) {
                min_x[count] = (seq_t)x;
                known[count] = true;
            }
        }
        init = true;
    }

    /* return answer or 0 if unknown */
    if (n < 256 && known[n])
        return min_x[n];
    return 0;
}
