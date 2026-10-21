/*
 * A068631  a(1) = 4; a(n) = smallest composite number different from the
 *          previous ones such that the sum of all the terms is a composite
 *          number.
 * https://oeis.org/A068631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Objective:  A068631 – a composite sequence where
 * a(1) = 4, and for n>1, a(n) is the smallest composite
 * not previously used such that the cumulative sum up to a(n)
 * is composite.
 *
 * Implementation uses a simple sieve for primality testing up
 * to a fixed limit and incremental search of candidates.
 * The function is memoised; earlier terms are stored and
 * reused for any order of calls.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* a(n) = smallest unused composite making the sum composite. */
seq_t a(idx_t n) {
    /* ---------- Local static data ------------ */
    static seq_t *seq = NULL;          /* sequence storage */
    static size_t seq_cap = 0;         /* current capacity */
    static size_t seq_len = 0;         /* filled length    */
    static seq_t sum = 0;              /* cumulative sum  */

    /* Sieve limits and storage. */
    static const uint64_t SIEVE_INIT = 2000000; /* 2 million is ample for n ≤ 10 000 */
    static bool *isprime = NULL;
    static bool *used = NULL;          /* marks composites already chosen */
    static uint64_t sieve_limit = 0;

    /* Ensure sequence storage is ready. */
    if (seq_len == 0) {
        seq_cap = 256;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(1);           /* abort if allocation fails */

        /* initialise sieve */
        sieve_limit = SIEVE_INIT;
        isprime = (bool*)malloc((sieve_limit + 1) * sizeof(bool));
        used   = (bool*)malloc((sieve_limit + 1) * sizeof(bool));
        if (!isprime || !used) exit(1);

        for (uint64_t i = 0; i <= sieve_limit; ++i) {
            isprime[i] = true;
            used[i]   = false;
        }
        isprime[0] = isprime[1] = false;
        for (uint64_t p = 2; p * p <= sieve_limit; ++p) {
            if (isprime[p]) {
                for (uint64_t m = p * p; m <= sieve_limit; m += p)
                    isprime[m] = false;
            }
        }
    }

    /* Helper to extend sieve if candidate is beyond current limit. */
    if (n > seq_len) { /* only when we need new terms */
        /* The following loop will generate all required terms. */
        for (;;) {
            /* ----------- find next term -------------- */
            uint64_t cand = 4;            /* start searching from 4 */
            for (;;) {
                /* extend sieve as needed */
                if (cand > sieve_limit) {
                    /* grow sieve roughly double */
                    uint64_t new_limit = sieve_limit * 2;
                    while (cand > new_limit) new_limit *= 2;
                    bool *new_prime = (bool*)malloc((new_limit + 1) * sizeof(bool));
                    bool *new_used   = (bool*)malloc((new_limit + 1) * sizeof(bool));
                    if (!new_prime || !new_used) exit(1);
                    for (uint64_t i = 0; i <= new_limit; ++i) {
                        new_prime[i] = true;
                        new_used[i]   = (i <= sieve_limit) ? used[i] : false;
                    }
                    new_prime[0] = new_prime[1] = false;
                    for (uint64_t p = 2; p * p <= new_limit; ++p) {
                        if (new_prime[p]) {
                            for (uint64_t m = p * p; m <= new_limit; m += p)
                                new_prime[m] = false;
                        }
                    }
                    free(isprime);
                    free(used);
                    isprime = new_prime;
                    used   = new_used;
                    sieve_limit = new_limit;
                }

                /* composite test */
                bool cand_is_prime;
                if (cand <= sieve_limit) cand_is_prime = isprime[cand];
                else {
                    /* fallback: trial division up to sqrt */
                    cand_is_prime = true;
                    for (uint64_t d = 2; d * d <= cand; ++d)
                        if (cand % d == 0) { cand_is_prime = false; break; }
                }
                if (cand_is_prime) { ++cand; continue; } /* not composite */

                if (used[(size_t)cand]) { ++cand; continue; } /* already used */

                /* check if sum+cand composite */
                seq_t new_sum = sum + (seq_t)cand;
                /* composite if >1 and not prime */
                bool new_sum_prime;
                if (new_sum <= (seq_t)sieve_limit) new_sum_prime = isprime[(size_t)new_sum];
                else {
                    uint64_t tmp = (uint64_t)new_sum;
                    new_sum_prime = true;
                    for (uint64_t d = 2; d * d <= tmp; ++d)
                        if (tmp % d == 0) { new_sum_prime = false; break; }
                }
                if (new_sum_prime) { ++cand; continue; } /* sum would be prime */

                /* found term */
                /* extend seq array if needed */
                if (seq_len == seq_cap) {
                    seq_cap *= 2;
                    seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
                    if (!seq) exit(1);
                }
                seq[seq_len++] = (seq_t)cand;
                used[(size_t)cand] = true;
                sum = new_sum;
                break; /* next term ready */
            }
            if (seq_len >= n) break;        /* we have enough terms */
        }
    }

    /* return requested term */
    return seq[n-1];
}
