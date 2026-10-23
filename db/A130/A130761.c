/*
 * A130761  Primes prime(n) such that at least one of the two numbers
 *          (prime(n+2)^2-prime(n)^2)/2 - 1 and (prime(n+2)^2-prime(n)^2)/2 +
 *          1 is prime.
 * https://oeis.org/A130761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 868, u32 n <= 55, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130761: primes p such that at least one of
   (prime(n+2)^2 - prime(n)^2)/2 +/- 1 is prime.
   Computed by incrementally generating primes and testing each
   triple of consecutive primes. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static uint64_t *primes = NULL;          /* list of primes (2,3,5,…)      */
static size_t   primes_len = 0;          /* number of primes stored      */
static size_t   primes_cap = 0;          /* capacity of primes array     */

static seq_t *seq = NULL;                /* qualified primes             */
static size_t  seq_len = 0;              /* number of terms stored       */
static size_t  seq_cap = 0;              /* capacity of seq array        */

/* grow the primes array */
static void grow_primes(void)
{
    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    primes     = tmp;
    primes_cap = new_cap;
}

/* grow the sequence array */
static void grow_seq(void)
{
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq     = tmp;
    seq_cap = new_cap;
}

/* add the next prime to the primes array and test the new
   triple of consecutive primes for the sequence condition.
   A qualified left prime is appended to seq. */
static void add_next_prime(void)
{
    uint64_t cand;
    if (primes_len == 0) {
        cand = 2;
    } else {
        cand = primes[primes_len - 1] + 1;
        if ((cand & 1ULL) == 0 && cand != 2) cand++;   /* skip even numbers except 2 */
    }

    /* find the next prime */
    while (true) {
        if (is_prime_big((seq_t)cand)) {
            /* store the new prime */
            if (primes_len == primes_cap) grow_primes();
            primes[primes_len++] = cand;

            /* check the triple that ends with this new prime */
            if (primes_len >= 3) {
                uint64_t left  = primes[primes_len - 3];
                uint64_t right = primes[primes_len - 1];

                uint64_t diff = right * right - left * left;
                if ((diff & 1ULL) == 0) {          /* diff must be even */
                    uint64_t base = diff / 2ULL;
                    bool ok = false;
                    if (base > 2 && is_prime_big((seq_t)(base - 1)))
                        ok = true;
                    else if (is_prime_big((seq_t)(base + 1)))
                        ok = true;

                    if (ok) {
                        if (seq_len == seq_cap) grow_seq();
                        seq[seq_len++] = (seq_t)left;
                    }
                }
            }
            break;   /* prime found, exit loop */
        }
        cand += 2;   /* test next odd candidate */
    }
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, guard clause */

    /* generate enough terms */
    while (seq_len < (size_t)n)
        add_next_prime();

    return seq[n - 1];   /* zero‑based storage */
}
