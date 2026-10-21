/*
 * A235991  Numbers with an odd arithmetic derivative, cf. A003415.
 * https://oeis.org/A235991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235991
   numbers n with n ≡ 2 (mod 4) or (n odd ∧ Ω(n) odd).
   The nth term is searched by brute‐force; a small prime table
   suffices for n up to several thousand. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes.h"

static idx_t *primes = NULL;      /* array of primes */
static size_t prime_cnt = 0;       /* number of primes stored */
static bool primes_inited = false;

/* initialise a static prime table sufficient for the problem size */
static void init_primes(void)
{
    if (!primes_inited) {
        const idx_t LIMIT = 200000;                /* > sqrt of candidate range */
        build_primes(LIMIT, &primes, &prime_cnt);
        primes_inited = true;
    }
}

/* total number of prime factors of x counting multiplicity (Ω(x)) */
static int omega(uint64_t x)
{
    int cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (p * p > x) break;
        while (x % p == 0) {
            ++cnt;
            x /= p;
        }
    }
    if (x > 1) ++cnt;     /* remaining prime factor > sqrt(original x) */
    return cnt;
}

/* external linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for offset 0 */

    init_primes();

    uint64_t cand = 1;          /* candidate integer */
    idx_t    found = 0;          /* how many terms found so far */

    for (;;) {
        /* condition: n ≡ 2 (mod 4)   or   (n odd and Ω(n) odd) */
        if ((cand % 4ULL) == 2ULL ||
            ((cand % 2ULL) == 1ULL && (omega(cand) & 1))) {
            ++found;
            if (found == n) return (seq_t)cand;
        }
        ++cand;
    }
}
