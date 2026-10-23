/*
 * A024470  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [
 *          (n+1)/2 ], s = (Lucas numbers), t = (1, p(1), p(2), ...).
 * https://oeis.org/A024470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{i=1}^{k} s(i) t(n+1-i), k = ⌊(n+1)/2⌋ where s(i) =
 *            Lucas numbers with L1 = 1, L2 = 3, t(1) = 1, t(j) = p(j-1)
 *            (primes)
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

static idx_t *prime_cache = NULL;   /* array of primes, 0‑based for p(1) */
static size_t prime_count = 0;      /* number of primes stored */

/* Ensure prime_cache contains at least 'needed' primes.   Prime indices start at 1. */
static void ensure_primes_idx(idx_t needed)
{
    if (needed <= prime_count) return;

    idx_t limit;
    if (needed < 6)
        limit = 15;                                       /* p5 = 11 */
    else {
        double n = (double)needed;
        double bound = n * (log(n) + log(log(n)));        /* upper bound for p_n, n>=6 */
        limit = (idx_t)(bound + 3.0);                    /* safety margin */
    }

    unsigned char *sieve = (unsigned char*)malloc(limit + 1);
    if (!sieve) exit(1);
    for (idx_t i = 0; i <= limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p)
        if (sieve[p])
            for (idx_t j = p * p; j <= limit; j += p)
                sieve[j] = 0;

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    idx_t *new_cache = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!new_cache) exit(1);
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i]) new_cache[idx++] = i;

    free(sieve);
    free(prime_cache);
    prime_cache = new_cache;
    prime_count = cnt;
}

/* Compute a(n) as defined.  n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset, but return 0 */

    idx_t k = (n + 1) / 2;          /* floor((n+1)/2) */

    if (n > 1)                       /* need primes up to p(n-1) */
        ensure_primes_idx(n - 1);

    seq_t sum = 0;
    seq_t l_prev = 1;                /* L1 */
    seq_t l_curr = 3;                /* L2 */

    for (idx_t i = 1; i <= k; ++i) {
        seq_t l_val;
        if (i == 1)
            l_val = l_prev;
        else if (i == 2)
            l_val = l_curr;
        else {
            seq_t l_next = l_prev + l_curr;  /* L(i) = L(i-1)+L(i-2) */
            l_prev = l_curr;
            l_curr = l_next;
            l_val = l_next;
        }

        idx_t tpos = n - i + 1;             /* position in t sequence */
        seq_t t_val;
        if (tpos == 1)
            t_val = 1;
        else
            t_val = (seq_t)prime_cache[tpos - 2];   /* primes stored 0‑based */

        sum += l_val * t_val;
    }

    return sum;
}
