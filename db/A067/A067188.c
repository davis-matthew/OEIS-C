/*
 * A067188  Numbers that can be expressed as the (unordered) sum of two primes
 *          in exactly two ways.
 * https://oeis.org/A067188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the n-th even number that can be written as the sum of
 *            two primes in exactly two unordered ways. The function generates
 *            these numbers on demand using a prime sieve and counts Goldbach
 *            representations
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
static idx_t sieve_limit = 0;      /* current sieve upper bound          */
static bool *is_prime = NULL;       /* prime flag array                   */
static uint64_t *primes_arr = NULL;/* list of primes up to sieve_limit   */
static size_t primes_cnt = 0;       /* number of primes in primes_arr     */

static seq_t *terms = NULL;         /* accumulated terms                  */
static size_t terms_cnt = 0;        /* number of terms stored             */
static idx_t current_limit = 0;     /* working upper bound for term search*/
static idx_t last_processed = 4;    /* next even number to examine        */

/* ------------------------------------------------------------------ */
/* Rebuild the sieve (and prime list) up to a new limit.               */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    bool *new_is_prime = (bool *)realloc(is_prime, (limit + 1) * sizeof(bool));
    if (!new_is_prime)
        return;                   /* allocation failure – ignore  */
    is_prime = new_is_prime;
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t p = 2; p <= limit / p; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = false;
        }
    }

    free(primes_arr);
    primes_arr = NULL;
    primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            uint64_t *tmp = (uint64_t *)realloc(primes_arr, (primes_cnt + 1) * sizeof(uint64_t));
            if (!tmp)
                break;          /* memory failure – stop  */
            primes_arr = tmp;
            primes_arr[primes_cnt++] = i;
        }
    }
    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Generate terms until we have at least 'require' entries.           */
static void generate_terms(idx_t require)
{
    if (current_limit == 0) {
        current_limit = 1000; /* initial working bound */
        last_processed  = 4;
    }

    while (terms_cnt < require) {
        ensure_sieve(current_limit);

        for (idx_t N = last_processed; N <= current_limit; N += 2) {
            int count = 0;
            for (size_t i = 0; i < primes_cnt && primes_arr[i] <= N / 2; ++i) {
                if (is_prime[N - primes_arr[i]]) {
                    ++count;
                    if (count > 2)
                        break;
                }
            }
            if (count == 2) {
                seq_t *tmp = (seq_t *)realloc(terms, (terms_cnt + 1) * sizeof(seq_t));
                if (!tmp)
                    return; /* allocation failure – cannot add more */
                terms = tmp;
                terms[terms_cnt++] = (seq_t)N;
            }
        }
        last_processed = current_limit + 2;
        if (terms_cnt >= require)
            break;
        current_limit *= 2;          /* double the search bound */
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function required by the specification.            */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for non‑positive indices */
        return 0;
    generate_terms(n);
    if (n > terms_cnt)   /* should not happen if the sequence is infinite */
        return 0;
    return terms[n - 1];
}
