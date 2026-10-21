/*
 * A057447  a(n+1) = next prime such that a(n+1)-1 | (a(1)...a(n))^3.
 * https://oeis.org/A057447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binary_search, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A057447
   a(1) = 2.
   For n > 1, a(n) is the smallest prime > a(n‑1) such that
   a(n)−1 | (a(1)⋯a(n‑1))³.  The implementation keeps the
   prime exponents of the product a(1)⋯a(k) in a sorted array.
   When a candidate prime p is found, p−1 is factored only against
   this array; an exponent e of a prime q in p−1 must satisfy
   3·exp_product(q) ≥ e.  No built‑in arbitrary precision arithmetic
   is required. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime.h"          /* bool is_prime(idx_t); */
#include "../utils/binary_search.h"

seq_t a(idx_t n)
{
    /* Static cache of already generated terms */
    static seq_t *seq   = NULL;   /* array of sequence terms (prime values)      */
    static size_t seq_len = 0;     /* number of terms stored (1‑based)           */
    static size_t seq_cap = 0;     /* allocated capacity in seq                   */

    /* Static data for the prime exponents of the product
       a(1) * … * a(k)  (k == seq_len).  The arrays are sorted by prime. */
    static uint64_t *prime   = NULL;   /* primes that appear in the product       */
    static unsigned *exp_cnt = NULL;   /* their current exponents in the product  */
    static size_t prime_len = 0;       /* number of distinct primes in the product*/
    static size_t prime_cap = 0;       /* allocated capacity in prime/exp_cnt      */

    /* Initialise on first invocation */
    if (seq_len == 0) {
        seq_cap = 64;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)2;
        seq_len = 1;

        prime_cap = 64;
        prime = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        exp_cnt = (unsigned*)malloc(prime_cap * sizeof(unsigned));
        prime[0] = 2;             /* prime 2 appears once in the product */
        exp_cnt[0] = 1;
        prime_len = 1;
    }

    /* Produce terms until the required index is reached */
    while (seq_len < n) {
        uint64_t prev = (uint64_t)seq[seq_len - 1];

        /* Start searching for a prime just above the previous term */
        uint64_t cand = prev + 1;
        if (cand <= 2) cand = 3;
        if (cand % 2 == 0) cand += 1;          /* make sure cand is odd */

        for (;;) {
            if (is_prime(cand)) {
                /* Verify that cand-1 divides (product)³.
                   For each prime factor q^e of cand-1 we must have
                   3*exp_cnt_of_q >= e. */
                uint64_t m = cand - 1;
                uint64_t temp = m;
                bool ok = true;

                /* factor 2 separately */
                if ((temp & 1) == 0) {
                    unsigned pe = 0;
                    while ((temp & 1) == 0) {
                        temp >>= 1;
                        ++pe;
                    }
                    int idx = binary_search(prime, prime_len, 2);
                    unsigned prodExp = (idx >= 0) ? exp_cnt[idx] : 0;
                    if (3 * prodExp < pe) ok = false;
                }

                /* factor odd primes */
                for (uint64_t f = 3; f * f <= temp; f += 2) {
                    if (temp % f == 0) {
                        unsigned pe = 0;
                        while (temp % f == 0) {
                            temp /= f;
                            ++pe;
                        }
                        int idx = binary_search(prime, prime_len, f);
                        unsigned prodExp = (idx >= 0) ? exp_cnt[idx] : 0;
                        if (3 * prodExp < pe) { ok = false; break; }
                    }
                }

                /* remaining prime factor (if any) */
                if (ok && temp > 1) {
                    int idx = binary_search(prime, prime_len, temp);
                    unsigned prodExp = (idx >= 0) ? exp_cnt[idx] : 0;
                    if (3 * prodExp < 1) ok = false;
                }

                if (ok) {
                    /* Accept candidate: store it and update data. */
                    if (seq_cap <= seq_len) {
                        seq_cap = seq_cap * 2 + 1;
                        seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
                    }
                    seq[seq_len++] = (seq_t)cand;

                    /* Add the new prime to the product exponent array.
                       Since candidates are searched in increasing order,
                       the array stays sorted. */
                    if (prime_cap <= prime_len) {
                        prime_cap = prime_cap * 2 + 1;
                        prime   = (uint64_t*)realloc(prime,   prime_cap * sizeof(uint64_t));
                        exp_cnt = (unsigned*)realloc(exp_cnt, prime_cap * sizeof(unsigned));
                    }
                    prime[prime_len] = cand;
                    exp_cnt[prime_len] = 1;
                    ++prime_len;

                    break;  /* go to next term   */
                }
            }

            /* Next candidate prime (skip even numbers) */
            cand += (cand == 2) ? 1 : 2;
        }
    }

    return seq[n - 1];
}
