/*
 * A080146  Binary encoding of quadratic residue set for each prime. a(n) =
 *          A055094(A000040(n)).
 * https://oeis.org/A080146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binary encoding of the set of nonzero quadratic residues
 *            modulo the n-th prime p, with bit at position (p-1 - r) set for
 *            each residue r. Thus a(n) = A055094(p)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation generates primes by trial division on demand and encodes residues as described. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime generation utilities (pure C, dynamic array of 32‑bit primes) */
static uint32_t *prime_list = NULL;
static size_t  prime_count = 0;
static size_t  prime_cap   = 0;

static void prime_add(uint32_t p)
{
    if (prime_count == prime_cap) {
        size_t newcap = prime_cap ? prime_cap * 2 : 64;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, newcap * sizeof(uint32_t));
        if (!tmp) exit(1);                     /* allocation failure */
        prime_list = tmp;
        prime_cap  = newcap;
    }
    prime_list[prime_count++] = p;
}

/* Ensure at least 'need' primes are generated (1‑based indexing). */
static void ensure_primes(size_t need)
{
    if (prime_count >= need) return;

    uint32_t cand = prime_count ? prime_list[prime_count - 1] + 1 : 2;

    while (prime_count < need) {
        bool prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) prime_add(cand);
        ++cand;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    ensure_primes((size_t)n);            /* have n primes in prime_list */

    uint32_t p = prime_list[n - 1];       /* n-th prime, 1‑based */

    seq_t result = (seq_t)0;
    const size_t bit_limit = sizeof(seq_t) * 8;

    /* Enumerate non‑zero quadratic residues: r = x^2 mod p, 1 ≤ x < p. */
    for (uint32_t x = 1; x < p; ++x) {
        uint32_t r = (uint32_t)((((uint64_t)x * x) % p));
        if (r == 0) continue;           /* skip residue 0 if present */

        uint32_t pos = p - 1 - r;        /* bit position (reversed order) */
        if (pos < bit_limit)
            result |= ((seq_t)1 << pos); /* set the corresponding bit */
    }

    return result;
}
