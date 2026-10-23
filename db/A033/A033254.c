/*
 * A033254  Primes of form x^2+85*y^2.
 * https://oeis.org/A033254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   the function finds primes of the form x^2 + 85*y^2 by using the
 *            known characterisation of their residues modulo 340: p ≡ 1, 9,
 *            21, 49, 69, 81, 89, 101, 121, 149, 161, 169, 189, 229, 281 or
 *            321 (mod 340). It keeps a cache of all such primes found so far,
 *            expanding the cache only when a request for...
 * Range:     u8 n <= 4, u16 n <= 785, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static bool residue_ok[340] = { 0 };

static void init_residue_ok(void)
{
    static bool done = false;
    if (done) return;
    const int ok_vals[16] = { 1, 9, 21, 49, 69, 81, 89, 101,
                              121, 149, 161, 169, 189, 229,
                              281, 321 };
    for (int i = 0; i < 16; ++i) residue_ok[ok_vals[i]] = true;
    done = true;
}

/* dynamic arrays of known primes (idx_t is unsigned 64-bit) */
static idx_t *prime_list = NULL;
static idx_t prime_cap  = 0;
static idx_t prime_cnt  = 0;

/* cache of sequence terms (seq_t may be an arbitrary-precision class */
static seq_t *seq_cache = NULL;
static idx_t seq_cap    = 0;
static idx_t seq_cnt    = 0;

/* next odd integer to be tested for primality */
static idx_t next_candidate = 3;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined only for n >= 1 */

    init_residue_ok();

    /* Ensure the prime list contains at least the first odd prime,
       this simplifies the trial division loop. */
    if (prime_cnt == 0) {
        prime_cap = 64;
        prime_list = (idx_t*)malloc(prime_cap * sizeof(idx_t));
        prime_list[prime_cnt++] = 3;
        prime_cap = 64;
    }

    /* Extend cache until the requested term is available. */
    while (seq_cnt < n) {
        idx_t cand = next_candidate;

        /* Trial division using the list of discovered primes. */
        bool is_prime = true;
        for (idx_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            if (p > cand / p) break;        /* p > sqrt(cand) */
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* Store the new prime in the prime list. */
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                prime_list = (idx_t*)realloc(prime_list,
                                             prime_cap * sizeof(idx_t));
            }
            prime_list[prime_cnt++] = cand;

            /* Keep it if it satisfies the residue condition. */
            if (residue_ok[cand % 340]) {
                if (seq_cnt == seq_cap) {
                    idx_t new_cap = seq_cap ? seq_cap << 1 : 64;
                    seq_cache = seq_realloc(seq_cache, seq_cap, new_cap);
                    seq_cap = new_cap;
                }
                seq_cache[seq_cnt++] = (seq_t)cand;
            }
        }

        next_candidate = cand + 2;   /* only odd candidates are needed */
    }

    return seq_cache[n - 1];
}
