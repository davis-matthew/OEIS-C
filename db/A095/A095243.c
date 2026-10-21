/*
 * A095243  a(n) = concatenation of 1,2,3,...,n mod prime(n).
 * https://oeis.org/A095243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = concatenation of 1,2,3,...,n modulo prime(n)
 * Range:     u8 n <= 54, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* local helper: return number of decimal digits of k (1 <= k <= 10^5 is enough) */
static int num_digits(idx_t k)
{
    if (k < 10)          return 1;
    if (k < 100)         return 2;
    if (k < 1000)        return 3;
    if (k < 10000)       return 4;
    if (k < 100000)      return 5;
    /* for larger k, fall back to log10 */
    return (int)(log10((double)k) + 1.0);
}

/* estimate an upper bound for the n-th prime (n >= 6) */
static idx_t estimate_upper_prime(idx_t n)
{
    if (n < 6) return 15;
    double dn = (double)n;
    double bound = dn * (log(dn) + log(log(dn)));
    return (idx_t)(bound + 3); /* safety margin */
}

/* generate all primes up to 'limit' and store them in *primes_out.
   On entry *cap_out contains current array capacity; on exit it may be updated.
   The returned count is in *cnt_out. */
static void build_primes(idx_t limit, idx_t **primes_out,
                         size_t *cnt_out, size_t *cap_out)
{
    /* free old data if any */
    if (*primes_out) {
        free(*primes_out);
        *primes_out = NULL;
        *cnt_out = 0;
    }

    /* linear sieve */
    unsigned char *is_comp = (unsigned char *)malloc((size_t)limit + 1);
    if (!is_comp) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    for (size_t i = 0; i <= (size_t)limit; ++i) is_comp[i] = 0;

    size_t estimated_cnt = limit / (log((double)limit) > 0 ? log((double)limit) : 1);
    *cap_out = estimated_cnt;
    *primes_out = (idx_t *)malloc(sizeof(idx_t) * (*cap_out));
    if (!*primes_out) {
        free(is_comp);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (cnt == *cap_out) {
                *cap_out *= 2;
                *primes_out = (idx_t *)realloc(*primes_out, sizeof(idx_t) * (*cap_out));
            }
            (*primes_out)[cnt++] = i;
        }
        for (size_t j = 0; j < cnt && i * (*primes_out)[j] <= limit; ++j) {
            is_comp[(size_t)(i * (*primes_out)[j])] = 1;
            if (i % (*primes_out)[j] == 0) break;
        }
    }
    free(is_comp);
    *cnt_out = cnt;
}

/* static state for primes */
static idx_t *prime_arr = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static idx_t sieve_limit = 0;

/* ensure primes array contains at least the first 'n' primes */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return; /* already enough */

    idx_t new_limit = estimate_upper_prime(n);
    if (new_limit <= sieve_limit) {
        /* current sieve already large enough */
        prime_cnt = 0; /* recompute count */
        for (idx_t i = 0; i < new_limit; ++i) {
            if (prime_arr[i] != 0) {
                if (prime_cnt == prime_cap) {
                    prime_cap *= 2;
                    prime_arr = (idx_t *)realloc(prime_arr, sizeof(idx_t) * prime_cap);
                }
                prime_arr[prime_cnt++] = i + 1; /* offset since array 0-based */
            }
        }
        return;
    }

    /* new sieve */
    free(prime_arr);
    prime_arr = NULL;
    prime_cap = 0;
    build_primes(new_limit, &prime_arr, &prime_cnt, &prime_cap);
    sieve_limit = new_limit;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but harmless */

    /* ensure we have primes up to n */
    ensure_primes(n);

    /* n-th prime */
    idx_t p = prime_arr[n - 1];
    seq_t mod = (seq_t)0;
    seq_t p_seq = (seq_t)p;

    for (idx_t k = 1; k <= n; ++k) {
        int d = num_digits(k);
        seq_t pow10 = (seq_t)1;
        for (int i = 0; i < d; ++i) pow10 *= (seq_t)10;
        mod = (mod * pow10 + (seq_t)k) % p_seq;
    }

    return mod;
}
