/*
 * A161180  Sum of primes between n^2 and n^2+n, n^2+n and (n+1)^2. The only
 *          intervals for which n^2+n is prime are [1,2], [2,4] and the
 *          endpoint 2 is included in the sum. One might have written
 *          0,3,5,7,..., etc.
 * https://oeis.org/A161180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the sum of primes that lie in the two intervals * [k²,
 *            k² + k] and [k² + k, (k + 1)²], where k = (n + 1) / 2 for odd n
 *            * and k = n / 2 for even n. The intervals are counted in order:
 *            * a(1) = sum in [1², 1²+1], a(2) = sum in [1²+1, 2²], a(3) = sum
 *            in [2², 2²+2], …
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* static storage for the sieve and prime list */
static char *prime_flag = NULL;       /* 0 = composite, 1 = prime */
static size_t prime_flag_size = 0;    /* current allocated size (limit + 1) */
static idx_t *prime_list   = NULL;    /* list of primes */
static size_t prime_count  = 0;       /* number of primes stored */
static size_t prime_cap    = 0;       /* capacity of prime_list */

/* current maximum sieve limit (inclusive) */
static idx_t current_limit = 0;

/* forward declarations */
static void add_prime(idx_t p);
static void extend_to(idx_t limit);
static void ensure_limit(idx_t limit);

/* add a prime to the list, resizing if necessary */
static void add_prime(idx_t p)
{
    if (prime_count == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
        if (!tmp) abort(); /* allocation failure */
        prime_list = tmp;
        prime_cap = new_cap;
    }
    prime_list[prime_count++] = p;
}

/* extend the sieve up to 'limit' (inclusive) */
static void extend_to(idx_t limit)
{
    if (limit <= current_limit) return;

    size_t old_size   = prime_flag_size;
    size_t new_size   = (size_t)limit + 1;
    char *tmp = (char *)realloc(prime_flag, new_size);
    if (!tmp) abort(); /* allocation failure */
    prime_flag = tmp;
    prime_flag_size = new_size;

    /* initially mark new entries as prime */
    if (new_size > old_size) {
        memset(prime_flag + old_size, 1, new_size - old_size);
    }

    /* forces 0 and 1 to be non‑prime */
    if (new_size > 0)   prime_flag[0] = 0;
    if (new_size > 1)   prime_flag[1] = 0;

    /* sieve the new range using already known primes */
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if ((idx_t)p * (idx_t)p > limit) break;
        idx_t start = (idx_t)p * (idx_t)p;
        if (start < (idx_t)(current_limit + 1)) {
            /* first multiple not below current_limit+1 */
            start = ((idx_t)(current_limit + 1) + p - 1) / p * p;
        }
        for (idx_t j = start; j <= limit; j += p) {
            prime_flag[(size_t)j] = 0;
        }
    }

    /* add newly discovered primes to the list */
    for (idx_t i = current_limit + 1; i <= limit; ++i) {
        if (prime_flag[(size_t)i]) {
            add_prime(i);
        }
    }

    current_limit = limit;
}

/* ensure that the sieve and prime list cover up to 'limit' */
static void ensure_limit(idx_t limit)
{
    if (limit <= current_limit) return;
    /* first grow to at least sqrt(limit) so that all needed primes are known */
    idx_t sqrt_lim = (idx_t)floor(sqrt((long double)limit));
    if (sqrt_lim > current_limit) {
        extend_to(sqrt_lim);
    }
    extend_to(limit);
}

/* the requested sequence function */
seq_t a(idx_t n)
{
    /* Determine k and interval bounds */
    idx_t k  = (n & 1) ? ((n + 1) >> 1) : (n >> 1);
    seq_t L, R;
    if (n & 1) {
        L = (seq_t)k * (seq_t)k;
        R = L + (seq_t)k;
    } else {
        L = (seq_t)k * (seq_t)k + (seq_t)k;
        R = (seq_t)(k + 1) * (seq_t)(k + 1);
    }

    /* Prepare sieve up to R */
    ensure_limit((idx_t)R);

    /* Sum primes in the interval [L,R] */
    seq_t sum = 0;
    for (idx_t x = (idx_t)L; x <= (idx_t)R; ++x) {
        if (prime_flag[(size_t)x]) {
            sum += (seq_t)x;
        }
    }
    return sum;
}
