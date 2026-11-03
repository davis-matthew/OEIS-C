/*
 * A177860  Product of the quadratic residues of prime(n).
 * https://oeis.org/A177860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product of the quadratic residues of prime(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static inline seq_t seq_one(void) { return (seq_t)1; }

/* Return the product of the quadratic residues modulo the n‑th prime. */
seq_t a(idx_t n)
{
    /* Prime corresponding to the index n. */
    uint32_t p = get_prime(n);

    /* Start product at 1. */
    seq_t prod = seq_one();

    /* For p = 2 the only residue is 1, so skip the loop. */
    if (p <= 2)
        return prod;

    /* Number of distinct non‑zero quadratic residues modulo an odd prime p
       is (p-1)/2. */
    idx_t half = (idx_t)((p - 1) / 2);

    /* Multiply the residues i^2 mod p for i = 1 .. (p-1)/2. */
    for (idx_t i = 1; i <= half; ++i) {
        /* i^2 modulo p fits in 32 bits. */
        uint32_t r = (uint32_t)(((uint64_t)i * i) % p);
        /* For integer seq_t this uses ordinary multiplication;
           for arbitrary‑precision seq_t the class overloads operator *. */
        prod = prod * (seq_t)r;
    }

    return prod;
}
