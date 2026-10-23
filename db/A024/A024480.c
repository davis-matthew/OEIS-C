/*
 * A024480  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [
 *          (n+1)/2 ], s = (1, p(1), p(2), ...), t = (composite numbers).
 * https://oeis.org/A024480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} s(i) · t(n+1-i) * where s = (1, p(1),
 *            p(2), …) and t = (composite numbers)
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *primes = NULL;   /* primes[0] = 2, primes[1] = 3, …   */
static size_t  primes_len = 0;
static seq_t *composites = NULL;  /* composites[0] = 4, composites[1] = 6, …   */
static size_t  composites_len = 0;
static uint64_t last_limit = 0;

/* Rebuild the prime and composite tables up to all numbers ≤ bound. */
static void rebuild_tables(uint64_t bound)
{
    free(primes);
    free(composites);

    /* Sieve of Eratosthenes.  is_prime[i] == 1  ⇔  i is prime.  */
    unsigned char *is_prime = (unsigned char *)calloc((size_t)(bound + 1), 1);
    if (!is_prime)
        return;

    /* Mark 2…bound as prime initially.  */
    for (uint64_t i = 2; i <= bound; ++i)
        is_prime[i] = 1;

    /* Cross out multiples of each prime.  */
    for (uint64_t i = 2; i * i <= bound; ++i) {
        if (!is_prime[i])
            continue;
        for (uint64_t j = i * i; j <= bound; j += i)
            is_prime[j] = 0;
    }

    /* Count primes and composites (≥4).  */
    size_t pcnt = 0, ccnt = 0;
    for (uint64_t i = 2; i <= bound; ++i) {
        if (is_prime[i])
            ++pcnt;
        else if (i >= 4)          /* composites start at 4 */
            ++ccnt;
    }

    primes = (uint64_t *)malloc(pcnt * sizeof(uint64_t));
    composites = (seq_t *)malloc(ccnt * sizeof(seq_t));
    if (!primes || !composites) {
        free(primes);
        free(composites);
        free(is_prime);
        return;
    }

    /* Fill the tables.  */
    size_t pidx = 0, cidx = 0;
    for (uint64_t i = 2; i <= bound; ++i) {
        if (is_prime[i]) {
            primes[pidx++] = i;
        } else if (i >= 4) {
            composites[cidx++] = (seq_t)i;
        }
    }

    primes_len        = pcnt;
    composites_len    = ccnt;
    last_limit        = bound;

    free(is_prime);
}

/* Ensure the tables are large enough for a request of size `needed`. */
static void ensure_tables(idx_t needed)
{
    /* If the existing tables already contain enough data, do nothing. */
    if (composites_len >= (size_t)needed &&
        primes_len     >= (size_t)((needed + 1) / 2))   /* primes needed: k-1 ≤ n-1 */
        return;

    /* Rough upper bound that guarantees at least `needed` composites and
       plenty of primes: 20×needed + 1000.   */
    uint64_t bound = (uint64_t)needed * 20ULL + 1000ULL;
    if (needed > last_limit)
        rebuild_tables(bound);
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined; guard against underflow. */
        return (seq_t)0;

    idx_t k = (n + 1) / 2;          /* number of terms in the sum. */

    ensure_tables(n);

    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t s;               /* s(i) */
        if (i == 1) {
            s = (seq_t)1;
        } else {
            /* i ≥ 2 → prime(i‑1) is primes[(i‑2)].  */
            size_t pindex = (size_t)(i - 2);
            s = (seq_t)primes[pindex];
        }

        idx_t tpos = n + 1 - i;                /* composite index (1‑based) */
        seq_t t = composites[(size_t)(tpos - 1)];

        sum += s * t;
    }

    return sum;
}
