/*
 * A194593  Semiprimes s such that phi(s)/2 is prime.
 * https://oeis.org/A194593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 282, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A194593:  semiprimes s such that phi(s)/2 is prime.
    a(1)=9, and for n>1  a(n)=2*p where p is the (n-1)-th safe prime
    (a prime p with (p-1)/2 also prime).
*/
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define SIEVE_LIMIT 5000000   /* sufficiently large for n up to ~10⁴ */

static bool initialized = false;
static seq_t *safe_primes = NULL;
static size_t safe_cnt = 0;

/* Build the list of safe primes up to SIEVE_LIMIT.  */
static void build_safe_primes(void)
{
    uint8_t *is_prime = (uint8_t *)malloc((size_t)SIEVE_LIMIT + 1);
    if (!is_prime) return;                       /* allocation failure */

    memset(is_prime, 1, (size_t)SIEVE_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= (size_t)SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)SIEVE_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* collect safe primes */
    size_t est_cap = 100;
    seq_t *sp = (seq_t *)malloc(est_cap * sizeof(seq_t));
    if (!sp) { free(is_prime); return; }

    size_t cnt = 0;
    for (size_t p = 3; p <= (size_t)SIEVE_LIMIT; p += 2) {
        if (!is_prime[p]) continue;
        size_t q = (p - 1) / 2;
        if (q >= 2 && is_prime[q]) {
            if (cnt == est_cap) {
                est_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(sp, est_cap * sizeof(seq_t));
                if (!tmp) { free(sp); free(is_prime); return; }
                sp = tmp;
            }
            sp[cnt++] = (seq_t)p;
        }
    }

    free(is_prime);

    /* replace previous list */
    free(safe_primes);
    safe_primes = sp;
    safe_cnt    = cnt;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)9;          /* first term */

    idx_t need = n - 1;                   /* we need the (n-1)-th safe prime */

    /* Build list lazily on first request */
    if (!initialized) {
        build_safe_primes();
        initialized = true;
    }

    /* If we have too few safe primes for the current request, grow the sieve.
       The fixed limit is sufficient for n up to about 10⁴, but we handle bigger
       requests by increasing the limit and rebuilding.               */
    if (safe_cnt < (size_t)need) {
        /* estimate a larger limit; double current limit for simplicity */
        size_t new_limit = SIEVE_LIMIT;
        while (safe_cnt < (size_t)need) {
            new_limit <<= 1;            /* double */
            /* Adjust global constant for next rebuild */
            /* (This affects only subsequent calls.) */
            /* rebuild with the new limit */
            uint8_t *is_prime = (uint8_t *)malloc(new_limit + 1);
            if (!is_prime) break;
            memset(is_prime, 1, new_limit + 1);
            is_prime[0] = is_prime[1] = 0;
            for (size_t i = 2; i * i <= new_limit; ++i) {
                if (is_prime[i]) {
                    for (size_t j = i * i; j <= new_limit; j += i)
                        is_prime[j] = 0;
                }
            }
            size_t est_cap = 100;
            seq_t *sp = (seq_t *)malloc(est_cap * sizeof(seq_t));
            if (!sp) { free(is_prime); break; }
            size_t cnt = 0;
            for (size_t p = 3; p <= new_limit; p += 2) {
                if (!is_prime[p]) continue;
                size_t q = (p - 1) / 2;
                if (q >= 2 && is_prime[q]) {
                    if (cnt == est_cap) {
                        est_cap <<= 1;
                        seq_t *tmp = (seq_t *)realloc(sp, est_cap * sizeof(seq_t));
                        if (!tmp) { free(sp); free(is_prime); break; }
                        sp = tmp;
                    }
                    sp[cnt++] = (seq_t)p;
                }
            }
            free(is_prime);
            free(safe_primes);
            safe_primes = sp;
            safe_cnt    = cnt;
            if (safe_cnt >= (size_t)need) break;
        }
    }

    if (safe_cnt < (size_t)need) return (seq_t)0; /* out of range */

    seq_t p = safe_primes[(size_t)need - 1];      /* (n-1)-th safe prime */
    return (seq_t)2 * p;
}
