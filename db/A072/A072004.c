/*
 * A072004  Remainder when sum of squares of first n primes is divided by n-th
 *          prime.
 * https://oeis.org/A072004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function computes a(n) = (sum of squares of first n primes) modulo the
 * n-th prime.  The sequence is defined for n ≥ 1; values for n ≤ 0 are
 * treated as zero.  This implementation keeps a prime list and a
 * running cumulative sum of squares, extending them only as far as the
 * largest n requested. */

#include <stdlib.h>
#include <stdint.h>

/* Static storage for primes and cumulative sums */
static idx_t *primes  = NULL;   /* primes[0] == 2, ...      */
static seq_t *sums    = NULL;   /* sums[i] == Σ_{k=1}^{i+1} p_k^2 */
static size_t prime_cnt = 0;    /* number of primes stored     */
static size_t prime_cap = 0;    /* allocated capacity          */
static seq_t current_sum = 0;   /* running sum of squares */

/* Grow the prime and sum arrays to at least 'new_cap' elements.  The
 * cast to (idx_t *) and (seq_t *) is required for C++ compilers. */
static void grow_arrays(size_t new_cap)
{
    idx_t *new_primes = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
    seq_t *new_sums   = (seq_t *)realloc(sums,   new_cap * sizeof(seq_t));
    primes = new_primes;
    sums   = new_sums;
    prime_cap = new_cap;
}

/* Extend the prime list and cumulative sums to contain at least n primes.
 * The algorithm uses trial division by all earlier primes.  For
 * efficiency it stops when the divisor exceeds the square root of the
 * candidate. */
static void extend_primes_to(size_t n)
{
    if (prime_cnt >= n) return;

    if (prime_cap == 0)           /* initial allocation */
        grow_arrays(16);

    while (prime_cnt < n) {
        idx_t candidate = (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 1;

        /* Find next prime by simple trial division */
        while (1) {
            int is_prime = 1;
            for (size_t i = 0; i < prime_cnt; ++i) {
                idx_t p = primes[i];
                if (p > candidate / p) break;      /* p² > candidate */
                if (candidate % p == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) break;   /* candidate is prime */
            ++candidate;
        }

        if (prime_cnt == prime_cap)            /* grow if needed */
            grow_arrays(prime_cap ? prime_cap * 2 : 16);

        primes[prime_cnt] = candidate;

        /* Square the candidate as seq_t to avoid overflow on small types */
        seq_t cs = (seq_t)candidate;
        cs *= cs;
        current_sum += cs;
        sums[prime_cnt] = current_sum;

        ++prime_cnt;
    }
}

/* Compute a(n).  Calls to this function may arrive in any order and
 * the internal state is memoised. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* behaviour below offset is irrelevant */
    extend_primes_to((size_t)n);
    idx_t p = primes[n - 1];
    seq_t s = sums[n - 1];
    return s % (seq_t)p;
}
