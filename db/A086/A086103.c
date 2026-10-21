/*
 * A086103  Primes appearing as the concatenation of the last two digits of
 *          prime(A086102(n)) and the first two digits of prime(A086102(n)+1).
 * https://oeis.org/A086103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   digit_len, get_prime, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A086103: primes that are concatenation of
   last two digits of prime(k) and first two digits of prime(k+1). */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

/* Utilities provided by the framework */
#include "../utils/get_prime.h"      /* seq_t get_prime(idx_t k) */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */
#include "../utils/pow10.h"          /* seq_t pow10(idx_t e) */
#include "../utils/digit_len.h"      /* int digit_len(seq_t x) */

#define MAX_CACHE 20000  /* safe upper bound for n <= ~10000 */

static seq_t cache[MAX_CACHE];   /* memoised a(n) values */
static size_t cache_cnt = 0;     /* number of elements stored */
static idx_t  gen_k   = 0;       /* current prime index processed (pair (k,k+1)) */

/* Return the nth term of the OEIS sequence A086103.  n >= 1.  */
seq_t a(idx_t n)
{
    /* Ensure we have at least n elements generated. */
    while (cache_cnt < n) {
        /* Process the next consecutive prime pair. */
        ++gen_k;                                 /* current prime index k */
        seq_t p_k    = get_prime(gen_k);          /* prime_k   */
        seq_t p_next = get_prime(gen_k + 1);      /* prime_{k+1} */

        /* Last two digits of p_k. */
        seq_t L = p_k % (seq_t)100;

        /* First two digits of p_next. */
        int dnext = digit_len(p_next); /* number of decimal digits of p_next */
        seq_t F;
        if (dnext > 1) {
            /* p_next has at least two digits. */
            seq_t pow = pow10(dnext - 2);
            F = p_next / pow;
        } else {
            /* p_next is a one–digit prime. */
            F = p_next;
        }

        /* Concatenated number: last two digits of p_k followed by first two of p_next. */
        seq_t concat = L * (seq_t)100 + F;

        /* If the concatenated number is prime, store it. */
        if (is_prime_big(concat)) {
            if (cache_cnt < MAX_CACHE) {
                cache[cache_cnt++] = concat;
            } else {
                /* Exceeded preallocated cache – should not happen for n <= ~10000. */
                /* If it does, we simply break; subsequent requests will fail. */
                break;
            }
        }
    }
    return cache[n - 1];
}
