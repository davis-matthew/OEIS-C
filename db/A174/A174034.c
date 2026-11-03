/*
 * A174034  The smallest prime p such that the double-concatenation prime(n)
 *          // prime(n+1) // p is a prime number.
 * https://oeis.org/A174034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the least prime p such that concat(prime(n), prime(n+1),
 *            p) is prime
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 1193, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9917 (2026-11-01)
 * Helpers:   is_prime_seq, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/num_digits_idx.h"

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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static seq_t concat_three(idx_t a, idx_t b, idx_t c)
{
    /* digits of b and c */
    idx_t db = num_digits_idx(b);
    idx_t dc = num_digits_idx(c);

    /* powers of 10 */
    seq_t pow_c   = pow10(dc);          /* 10^dc   */
    seq_t pow_bc  = pow10(db + dc);     /* 10^(db+dc) */

    /* b*10^dc + c */
    seq_t part_bc = (seq_t)b * pow_c + c;

    /* a*10^(db+dc) + (b*10^dc + c) */
    return (seq_t)a * pow_bc + part_bc;
}

seq_t a(idx_t n)
{
    /* 1‑based indexing */
    if (n == 0) return 0;           /* not used by the sequence, safe guard */

    idx_t prime_n     = get_prime(n);
    idx_t prime_nplus = get_prime(n + 1);

    /* search for the smallest prime p with the required property */
    for (idx_t p_idx = 1; ; ++p_idx)
    {
        idx_t p = get_prime(p_idx);
        seq_t candidate = concat_three(prime_n, prime_nplus, p);
        if (is_prime_seq(candidate))
            return (seq_t)p;
    }
}
