/*
 * A040061  Primes p such that x^3 = 11 has no solution mod p.
 * https://oeis.org/A040061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Primes p such that the congruence x^3 = 11 has no solution mod
 *            p. The function searches primes in increasing order and returns
 *            the nth such prime
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* not used, but harmless */

    /* Cache of generated sequence values */
    static seq_t *seq_cache = NULL;
    static size_t seq_cap = 0;
    static size_t seq_cnt = 0;

    /* Candidate for next prime to test */
    static uint64_t test_candidate = 2;

    /* Generate terms until we have at least n */
    while (seq_cnt < n) {
        /* Find the next prime number */
        while (1) {
            uint64_t p = test_candidate;
            ++test_candidate;

            if (p < 2) continue;

            /* Quick primality test */
            bool prime = true;
            if (p % 2 == 0) {
                prime = (p == 2);
            } else {
                for (uint64_t d = 3; d <= p / d; d += 2) {
                    if (p % d == 0) {
                        prime = false;
                        break;
                    }
                }
            }
            if (!prime) continue;

            /* We only care about primes with p ≡ 1 (mod 3) */
            if (p % 3 != 1) continue;

            /* Compute 11^((p-1)/3) mod p */
            uint64_t exp  = (p - 1) / 3;
            uint64_t base = 11 % p;
            uint64_t res  = 1;
            while (exp) {
                if (exp & 1) res = (unsigned __int128)res * base % p;
                base = (unsigned __int128)base * base % p;
                exp >>= 1;
            }
            if (res == 1) continue;       /* cubic residue -> skip */

            /* This prime qualifies; store it */
            if (seq_cnt == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;   /* allocation failure */
                seq_cache = tmp;
                seq_cap   = new_cap;
            }
            seq_cache[seq_cnt++] = (seq_t)p;
            break;      /* proceed to generate next needed term */
        }
    }

    return seq_cache[n - 1];
}
