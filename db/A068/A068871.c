/*
 * A068871  Duplicate of A004615.
 * https://oeis.org/A068871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the n-th integer ≥1 that is ≡1 (mod 10) and is either 1,
 *            a prime, or the square of a prime. (The first term is a(1)=1.)
 * Range:     i8 n <= 8, i16 n <= 19, i32 n <= 19, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Static storage for the generated sequence and the prime sieve */
static seq_t *seq_vals = NULL;   /* array of a(k) values */
static size_t seq_len = 0;       /* number of values stored */

static bool *prime_sieve = NULL; /* is_prime[i] for i ≤ sieve_limit */
static idx_t  sieve_limit = 0;   /* current upper bound of the sieve */

/* Build a prime sieve up to 'limit' and simultaneously compute the
   array of all numbers≡1 (mod 10) that are prime or have a prime square.   */
static void build_sequence(idx_t limit)
{
    /* --- 1.  Sieve of Eratosthenes ------------------------------------------------ */
    if (limit > sieve_limit) {
        /* Allocate new sieve */
        bool *sieve = (bool *)malloc((limit + 1) * sizeof(bool));
        for (idx_t i = 0; i <= limit; ++i) sieve[i] = true;
        if (limit >= 0) sieve[0] = false;
        if (limit >= 1) sieve[1] = false;

        for (idx_t p = 2; p * p <= limit; ++p) {
            if (sieve[p]) {
                for (idx_t j = p * p; j <= limit; j += p)
                    sieve[j] = false;
            }
        }

        /* Replace old sieve */
        free(prime_sieve);
        prime_sieve = sieve;
        sieve_limit = limit;
    }

    /* --- 2.  Mark squares of primes that end in 1 or 9 -------------------------- */
    bool *is_sq_of_prime = (bool *)calloc((limit + 1), sizeof(bool));
    for (idx_t p = 2; p <= limit; ++p) {
        if (prime_sieve[p] && (p % 10 == 1 || p % 10 == 9)) {
            if (p <= limit / p) {               /* avoid overflow */
                idx_t sq = p * p;
                if (sq <= limit)
                    is_sq_of_prime[sq] = true;
            }
        }
    }

    /* --- 3.  Form the list of required numbers ----------------------------------- */
    size_t est_cnt = limit / 10 + 10;            /* rough estimation */
    seq_t *new_vals = (seq_t *)malloc(est_cnt * sizeof(seq_t));
    size_t count = 0;

    for (idx_t x = 1; x <= limit; ++x) {
        /* 1 is always the first term */
        if (x == 1 ||
            ((x % 10 == 1) &&
             (prime_sieve[x] || is_sq_of_prime[x]))) {
            new_vals[count++] = (seq_t)x;
        }
    }

    /* --- 4.  Update global sequence ------------------------------------------------*/
    free(is_sq_of_prime);
    free(seq_vals);
    seq_vals = new_vals;
    seq_len  = count;
}

/* Public function requested by OEIS */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe return */

    /* Ensure we have computed enough terms */
    if (seq_len < n) {
        idx_t limit = (sieve_limit > 0) ? sieve_limit * 2 : 1024;
        if (limit < 1024) limit = 1024;
        while (seq_len < n) {
            build_sequence(limit);
            limit *= 2;              /* enlarge for next attempt if necessary */
        }
    }
    return seq_vals[n - 1];
}
