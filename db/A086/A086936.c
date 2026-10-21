/*
 * A086936  Number of primes between n and p(n) inclusive.
 * https://oeis.org/A086936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - π(n‑1) (number of primes between n and its nth prime
 *            inclusive)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build system. */

/* Cached list of primes, 1‑based. */
static idx_t *primes = NULL;        /* primes[0 .. primes_len-1]   */
static size_t primes_len = 0;       /* current number of primes    */
static size_t primes_cap = 0;       /* allocated capacity          */
static idx_t  max_checked = 0;      /* largest integer already tested */

/* Extends the prime cache so that all primes ≤ limit are known. */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= max_checked)
        return;               /* already done */

    /* Grow the array if necessary. */
    if (primes_len + 1 > primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 8;
        idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        if (!tmp) return;     /* allocation failed – future call will retry */
        primes = tmp;
        primes_cap = new_cap;
    }

    /* Test each integer from max_checked+1 up to limit. */
    idx_t start = max_checked + 1;
    if (start < 2) start = 2;          /* skip 0 and 1, which are not prime */
    for (idx_t cand = start; cand <= limit; ++cand) {
        int is_prime = 1;
        for (size_t i = 0; i < primes_len && primes[i] <= cand / primes[i]; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            primes[primes_len++] = cand;
        }
    }
    max_checked = limit;
}

/* Counts how many cached primes are ≤ x. */
static size_t count_primes_leq(idx_t x)
{
    if (primes_len == 0 || primes[0] > x)
        return 0;
    size_t left = 0, right = primes_len;      /* [left, right) */
    while (left < right) {
        size_t mid = (left + right) >> 1;
        if (primes[mid] <= x)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence offset is 1; undefined for n<1 */
        return 0;

    idx_t bound = n - 1;              /* we need π(n‑1) */
    ensure_primes_up_to(bound);
    size_t pi = count_primes_leq(bound);     /* π(n‑1) */

    seq_t pi_seq = (seq_t)pi;
    seq_t n_seq  = (seq_t)n;
    return n_seq - pi_seq;            /* a(n) = n – π(n‑1) */
}
