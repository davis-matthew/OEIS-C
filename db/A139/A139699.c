/*
 * A139699  Positive integers x,y such that prime(x)*prime(x+y) = y^2 + 1,
 *          ordered by increasing x+y.
 * https://oeis.org/A139699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, more
 * Formula:   This function computes OEIS sequence A139699. It searches for
 *            all pairs (x,y) satisfying prime(x)*prime(x+y)=y^2+1, orders the
 *            pairs by increasing s=x+y, and returns the n‑th element of the
 *            interleaved list x1,y1,x2,y2,
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* dynamically generated list of primes up to a fixed value */
#define PRIME_SIEVE_LIMIT 10000000ULL  /* 10 million is plenty for n≈10 000 */

static uint32_t *global_primes = NULL;
static size_t     global_prime_count = 0;

static void init_primes(void)
{
    if (global_primes != NULL)
        return;

    size_t limit = PRIME_SIEVE_LIMIT;
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return; /* allocation failed, caller will handle zero result */

    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++count;

    global_primes = (uint32_t *)calloc(count, sizeof(uint32_t));
    if (!global_primes) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) global_primes[idx++] = (uint32_t)i;

    global_prime_count = count;
    free(is_prime);
}

struct Pair {
    uint64_t x;
    uint64_t y;
    uint64_t sum;
};

static int cmp_pair(const void *a, const void *b)
{
    const struct Pair *pa = (const struct Pair *)a;
    const struct Pair *pb = (const struct Pair *)b;
    if (pa->sum < pb->sum) return -1;
    if (pa->sum > pb->sum) return 1;
    if (pa->x < pb->x) return -1;
    if (pa->x > pb->x) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    init_primes();
    if (global_primes == NULL) return 0; /* failed to initialise */

    size_t pairs_needed = (size_t)((n + 1) / 2); /* ceil(n/2) pairs */
    struct Pair *pairs = NULL;
    size_t pairs_cap = 0;
    size_t pairs_sz  = 0;

    /* helper to add a pair */
    if (pairs_needed > 0) {
        pairs_cap = pairs_needed * 2;
        pairs = (struct Pair *)malloc(pairs_cap * sizeof(struct Pair));
        if (!pairs) return 0;
    }

    for (uint64_t x = 1; pairs_sz < pairs_needed; ++x) {
        if ((size_t)x > global_prime_count) break; /* not enough primes */

        uint32_t p = global_primes[x - 1];

        for (uint64_t s = x + 1; ; ++s) {
            if ((size_t)s > global_prime_count) break; /* no more primes to test */

            uint32_t q = global_primes[s - 1];
            unsigned __int128 prod = (unsigned __int128)p * q;
            uint64_t y = s - x;
            unsigned __int128 rhs = (unsigned __int128)y * y + 1;

            if (prod == rhs) {
                if (pairs_sz >= pairs_cap) {
                    pairs_cap = pairs_cap * 2 + 1;
                    struct Pair *tmp = (struct Pair *)realloc(pairs, pairs_cap * sizeof(struct Pair));
                    if (!tmp) {
                        free(pairs);
                        return 0;
                    }
                    pairs = tmp;
                }
                pairs[pairs_sz].x   = x;
                pairs[pairs_sz].y   = y;
                pairs[pairs_sz].sum = s;
                ++pairs_sz;
                if (pairs_sz >= pairs_needed) break;
            }
            if (prod < rhs) break; /* further s will only increase rhs more */
        }
    }

    /* Sort pairs by increasing sum to keep correct order */
    if (pairs_sz > 1)
        qsort(pairs, pairs_sz, sizeof(struct Pair), cmp_pair);

    size_t idx = (size_t)((n - 1) / 2);
    seq_t result = 0;
    if (idx < pairs_sz) {
        if (n & 1)    result = (seq_t)pairs[idx].x;
        else          result = (seq_t)pairs[idx].y;
    }

    free(pairs);
    return result;
}
