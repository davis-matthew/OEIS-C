/*
 * A045393  Primes congruent to {0, 1, 3, 4, 5, 6} mod 7.
 * https://oeis.org/A045393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: incrementally generate primes and return those that
 * are not congruent to 2 modulo 7 (i.e., primes in the congruence class
 * {0,1,3,4,5,6} mod 7).  */
/* ABC: is_prime_cand() uses trial division with previously found primes. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *seq = NULL;          /* cache of sequence values   */
static size_t seq_cap = 0;         /* allocated capacity for seq */
static size_t seq_len = 0;         /* number of stored values    */

static uint64_t *primes = NULL;    /* list of primes found so far*/
static size_t primes_cap = 0;      /* allocated capacity for primes */
static size_t primes_len = 0;      /* number of stored primes     */

static uint64_t last_checked = 1;  /* last integer tested for primality */

static bool is_prime_cand(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p > n / p) break;          /* avoid overflow on p*p      */
        if (n % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n = 0      */

    while (seq_len < (size_t)n) {
        uint64_t cand = ++last_checked;
        bool prime = is_prime_cand(cand);

        if (prime) {
            /* store prime for future divisibility checks  */
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 4;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = cand;

            /* accept the candidate if it is not 2 mod 7  */
            if (cand % 7 != 2) {
                if (seq_len == seq_cap) {
                    size_t new_cap = seq_cap ? seq_cap * 2 : 4;
                    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                    seq = tmp;
                    seq_cap = new_cap;
                }
                seq[seq_len++] = (seq_t)cand;
            }
        }
    }

    return seq[n - 1];
}
