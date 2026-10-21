/*
 * A054735  Sums of twin prime pairs.
 * https://oeis.org/A054735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the n‑th twin prime pair (p, p+2)
 * Range:     u8 n <= 10, u16 n <= 505, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a simple sieve to generate twin primes up to a
   sufficient limit.  The routine is memoised so repeated calls are
   efficient. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *twin_sums      = NULL;   /* array of twin‑prime sums          */
static size_t twin_sum_len   = 0;       /* number of stored sums             */
static size_t twin_sum_cap   = 0;       /* allocated capacity                */
static idx_t  current_limit  = 0;       /* primes sieved up to this value    */

/* --------------------------------------------------------------------- */
/* Grow the twin_sums array to at least 'min_cap' elements.              */
static void grow_twin_sums(size_t min_cap) {
    if (min_cap <= twin_sum_cap) return;

    size_t new_cap = twin_sum_cap ? twin_sum_cap * 2 : 16;
    while (new_cap < min_cap) new_cap *= 2;

#if defined(__cplusplus)
    seq_t *new_ptr = new seq_t[new_cap];
    if (twin_sums) {
        for (size_t i = 0; i < twin_sum_len; ++i) new_ptr[i] = twin_sums[i];
        delete[] twin_sums;
    }
    twin_sums = new_ptr;
#else
    seq_t *new_ptr = (seq_t *)realloc(twin_sums, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();     /* out of memory */
    twin_sums = new_ptr;
#endif
    twin_sum_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Re‑sieve primes up to 'limit' and recompute all twin‑prime sums.      */
static void recompute_up_to(idx_t limit) {
    /* Allocate bitset for primes.  char is used so that the code is
       compatible with both C and C++. */
    char *is_prime = (char *)malloc((size_t)(limit + 1));
    if (!is_prime) abort();

    for (size_t i = 0; i <= (size_t)limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_prime[(size_t)j] = 0;
            }
        }
    }

    twin_sum_len = 0;
    grow_twin_sums(limit / 10 + 10);  /* rough estimate of expected count */

    for (idx_t p = 2; p + 2 <= limit; ++p) {
        if (is_prime[(size_t)p] && is_prime[(size_t)(p + 2)]) {
            seq_t sum = (seq_t)p + (seq_t)(p + 2);
            if (twin_sum_len == twin_sum_cap) grow_twin_sums(twin_sum_cap * 2);
            twin_sums[twin_sum_len++] = sum;
        }
    }

    free(is_prime);
    current_limit = limit;
}

/* --------------------------------------------------------------------- */
/* Ensure that the first 'n' twin‑prime sums are available.              */
static void ensure_terms(idx_t n) {
    if (twin_sum_len >= n) return;

    idx_t new_limit = current_limit ? current_limit * 2 : 1000;
    if (new_limit < 1000) new_limit = 1000;

    while (true) {
        recompute_up_to(new_limit);
        if (twin_sum_len >= n) break;
        new_limit *= 2;
    }
}

/* --------------------------------------------------------------------- */
/* Public interface: return a(n).  a(1) is the first twin‑prime sum.     */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* behaviour below the offset is unused */
    ensure_terms(n);
    return twin_sums[n - 1];
}
