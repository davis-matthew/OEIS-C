/*
 * A071567  Smallest k>n such that n^2+n+1 divides k^2+k+1.
 * https://oeis.org/A071567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k > n such that n^2+n+1 | k^2+k+1. The
 *            implementation: • Compute m = n^2+n+1. • Factor m using a simple
 *            sieve. • For each prime power p^e dividing m, find all solutions
 *            r to r^2 + r + 1 ≡ 0 (mod p^e) by brute‑force Hensel lifting. •
 *            Combine the local solutions with the CRT to obta...
 * Range:     u8 n <= 16, u16 n <= 258, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   crt_combine, egcd_inv, root_mod_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/crt_combine.h"
#include "../utils/egcd_inv.h"
#include "../utils/root_mod_prime_power.h"

/* Local primes up to 100000 for factorisation */
#define PRIME_MAX 100000
static bool primes_built = false;
static unsigned primes[12000];
static size_t primes_cnt;

static void build_primes(void)
{
    if (primes_built) return;
    bool mark[PRIME_MAX + 1];
    for (size_t i = 0; i <= PRIME_MAX; ++i) mark[i] = true;
    mark[0] = mark[1] = false;
    for (size_t i = 2; i * i <= PRIME_MAX; ++i)
        if (mark[i])
            for (size_t j = i * i; j <= PRIME_MAX; j += i)
                mark[j] = false;
    size_t n = 0;
    for (size_t i = 2; i <= PRIME_MAX; ++i)
        if (mark[i]) primes[n++] = (unsigned)i;
    primes_cnt = n;
    primes_built = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset starts at 1, undefined below 1 */
    uint64_t N = (uint64_t)n;
    uint64_t m = N * N + N + 1U;       /* modulus */

    build_primes();

    uint64_t tmp = m;
    uint64_t current_mod = 1;                /* product of processed prime powers */
    uint64_t residues[256];                  /* solutions mod current_mod */
    size_t residue_cnt = 0;

    /* helper arrays for temporary roots of a prime power */
    uint64_t temp_roots[256];
    size_t temp_cnt;

    /* factorisation of m */
    for (size_t pi = 0; pi < primes_cnt && (uint64_t)primes[pi] * primes[pi] <= tmp; ++pi) {
        uint64_t p = primes[pi];
        if (tmp % p != 0) continue;
        unsigned e = 0;
        uint64_t pe = 1;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
            pe *= p;
        }
        /* find roots modulo p^e */
        root_mod_prime_power(p, e, temp_roots, &temp_cnt);
        if (temp_cnt == 0) return 0;          /* should not happen */

        if (residue_cnt == 0) {
            /* first factor: just copy */
            for (size_t i = 0; i < temp_cnt; ++i)
                residues[i] = temp_roots[i];
            residue_cnt = temp_cnt;
        } else {
            /* combine previous residues with new roots */
            uint64_t new_res[256];
            size_t new_cnt = 0;
            for (size_t i = 0; i < residue_cnt; ++i) {
                uint64_t r1 = residues[i];
                for (size_t j = 0; j < temp_cnt; ++j) {
                    uint64_t r2 = temp_roots[j];
                    uint64_t r = crt_combine(r1, current_mod, r2, pe);
                    new_res[new_cnt++] = r;
                }
            }
            for (size_t i = 0; i < new_cnt; ++i)
                residues[i] = new_res[i];
            residue_cnt = new_cnt;
        }
        current_mod *= pe;   /* now current_mod == product of processed prime powers */
    }

    if (tmp > 1) {   /* remaining prime factor > sqrt(original m) */
        uint64_t p = tmp;   /* exponent e = 1 */
        root_mod_prime_power(p, 1, temp_roots, &temp_cnt);
        if (temp_cnt == 0) return 0;
        if (residue_cnt == 0) {
            for (size_t i = 0; i < temp_cnt; ++i)
                residues[i] = temp_roots[i];
            residue_cnt = temp_cnt;
            current_mod = p;
        } else {
            uint64_t new_res[256];
            size_t new_cnt = 0;
            for (size_t i = 0; i < residue_cnt; ++i) {
                uint64_t r1 = residues[i];
                for (size_t j = 0; j < temp_cnt; ++j) {
                    uint64_t r2 = temp_roots[j];
                    uint64_t r = crt_combine(r1, current_mod, r2, p);
                    new_res[new_cnt++] = r;
                }
            }
            for (size_t i = 0; i < new_cnt; ++i)
                residues[i] = new_res[i];
            residue_cnt = new_cnt;
            current_mod *= p;   /* should equal m */
        }
    }

    /* current_mod should be exactly m */
    uint64_t best = 0;
    bool first = true;
    for (size_t i = 0; i < residue_cnt; ++i) {
        uint64_t r = residues[i] % m;
        uint64_t k;
        if (r > N) {
            k = r;
        } else {
            uint64_t diff = N - r;
            uint64_t add = diff / m + 1;
            k = r + add * m;
        }
        if (first || k < best) {
            best = k;
            first = false;
        }
    }
    return (seq_t)best;
}
