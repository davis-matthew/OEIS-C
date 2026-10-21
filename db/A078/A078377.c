/*
 * A078377  Number of distinct prime factors of numbers which can be written
 *          as sum of a positive square and a positive cube.
 * https://oeis.org/A078377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ω(A055394(n)) Number of distinct prime factors of the
 *            n‑th integer that can be written as a positive square plus a
 *            positive cube
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*------------------------------------------------------------*/
/* Count distinct prime factors of a 64‑bit integer.  No utility */
/* header is required – a small trial‑division routine is used. */
static unsigned count_distinct_prime_factors(uint64_t n) {
    if (n < 2) return 0;
    unsigned cnt = 0;
    /* factor 2 */
    if ((n & 1ULL) == 0) {
        cnt++;
        while ((n & 1ULL) == 0) n >>= 1;
    }
    /* odd factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) cnt++;
    return cnt;
}
/*------------------------------------------------------------*/

static seq_t *cache = NULL;      /* cache of ω(A055394(k)) values */
static size_t cache_len = 0;     /* number of cached values */

/* Build the cache once.  The chosen maximum value (5 000 000) is large
   enough to contain more than 30 000 representable numbers, which covers
   the first 10 000 terms required. */
static void build_cache(void) {
    const size_t MAX_VAL = 5000000;
    /* Mark numbers that can be expressed as a^2 + b^3 */
    char *mark = (char *)calloc(MAX_VAL + 1, 1);
    if (!mark) exit(EXIT_FAILURE);
    for (size_t a = 1; a * a <= MAX_VAL; ++a) {
        uint64_t a2 = (uint64_t)a * a;
        for (size_t b = 1;; ++b) {
            uint64_t c = (uint64_t)b * b * b;
            uint64_t v = a2 + c;
            if (v > MAX_VAL) break;
            mark[v] = 1;
        }
    }

    /* Count how many marks there are */
    size_t cnt = 0;
    for (size_t i = 1; i <= MAX_VAL; ++i) if (mark[i]) ++cnt;

    /* Allocate array for the representable numbers */
    uint64_t *nums = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!nums) exit(EXIT_FAILURE);
    size_t idx = 0;
    for (size_t i = 1; i <= MAX_VAL; ++i) if (mark[i]) nums[idx++] = i;
    free(mark);

    /* Allocate cache array */
    cache = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);
    for (size_t i = 0; i < cnt; ++i) {
        cache[i] = (seq_t)count_distinct_prime_factors(nums[i]);
    }
    cache_len = cnt;
    free(nums);
}

/* Return the n‑th term, with 1‑based indexing.  The function is
   safe for any n ≥ 1; behaviour for n == 0 is unspecified but
   returned as 0. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* outside the offset – ignore */
    if (cache_len == 0) build_cache();
    if ((idx_t)cache_len >= n) return cache[n - 1];
    /* In practice this should never happen for the requested ranges. */
    return 0;
}
