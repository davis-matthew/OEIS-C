/*
 * A118835  Numerators of n-th convergent to continued fraction with semiprime
 *          terms.
 * https://oeis.org/A118835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = numerator of the n‑th convergent of the continued
 *            fraction [4; 6, 9, 10, 14, ...] which uses the semiprimes
 *            A001358 (except the first one 4)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required for memory management and integer types. */
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* Cache of the first N semiprimes (1‑based indices). */
static uint32_t *semis_cache = NULL;
static size_t    semis_cache_len = 0;

/* Build the cache of the first `need` semiprimes. */
static void build_semis(size_t need)
{
    if (semis_cache_len >= need)
        return;                         /* already have enough */

    /* Clean old cache */
    free(semis_cache);
    semis_cache_len = need;

    /* A heuristic upper bound for the largest number that must be examined
       to obtain `need` semiprimes.  The bound is generous for `need < 2·10⁴`. */
    size_t limit = need * 10 + 100;

    /* Smallest prime factor sieve (linear time). */
    uint32_t *spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                       /* i is prime */
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
            }
        }
    }

    /* Collect the first `need` semiprimes. */
    semis_cache = (uint32_t *)malloc((need + 1) * sizeof(uint32_t)); /* 1‑based */
    size_t cnt = 0;
    for (size_t m = 2; m <= limit && cnt < need; ++m) {
        size_t x = m;
        int    count = 0;
        while (x > 1 && count <= 2) {
            uint32_t p = spf[x];
            ++count;
            x /= p;
        }
        if (count == 2) {                         /* exactly two prime factors */
            ++cnt;
            semis_cache[cnt] = (uint32_t)m;
        }
    }
    free(spf);
    assert(cnt == need);
}

/* ------------------------------------------------------------ */
/* Sequence a(n) – numerator of the n‑th convergent.           */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, guard against misuse */

    /* Ensure we have enough semiprimes cached. */
    build_semis((size_t)n);

    /* Continuant recurrence:  a(1) = 4, a(0) = 1 (for the recurrence). */
    seq_t prev2 = (seq_t)1;          /* a(0)   */
    seq_t prev1 = (seq_t)4;          /* a(1)   */

    for (idx_t i = 1; i < n; ++i) {
        seq_t b   = (seq_t)semis_cache[(size_t)(i + 1)];   /* 4 is at index 1, but we start with 6 */
        seq_t curr = b * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;   /* numerator for the n‑th convergent */
}
