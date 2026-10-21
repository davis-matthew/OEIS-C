/*
 * A074668  Differences between successive 7-digit distinct-digit primes.
 * https://oeis.org/A074668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, base
 * Formula:   a(n) = difference between the (n+1)-th and the n-th 7‑digit
 *            primes that contain 7 distinct digits
 * Range:     u8 n <= 5, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Forward declarations of utilities that may be provided by the
   build environment.  The file names follow the convention
   "../utils/NAME.h".  We need seq_alloc to construct an array of
   seq_t correctly for all supported builds, including arbitrary‑
   precision C++ builds. */
#include "../utils/seq_alloc.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Check whether all decimal digits of a 7‑digit integer are distinct. */
static bool distinct_digits(uint32_t n)
{
    bool used[10] = {false};
    while (n > 0) {
        uint32_t d = n % 10;
        if (used[d]) return false;
        used[d] = true;
        n /= 10;
    }
    return true;
}

/* Static cache state. */
static bool          g_built      = false;
static uint32_t     *g_primes      = NULL;
static size_t        g_prime_cnt   = 0;
static seq_t        *g_diffs      = NULL;
static size_t        g_diff_cnt    = 0;

/* Build all 7‑digit primes with distinct digits and their successive
   differences in a single run.  The routine is idempotent. */
static void build(void)
{
    if (g_built) return;
    g_built = true;

    /* 7‑digit numbers: 1 000 000 … 9 999 999.  The sieve limit must
       include the largest such number.  We choose LIMIT = 10 000 000
       (one beyond the largest 7‑digit number). */
    const uint32_t LIMIT = 10000000U;   /* one past the largest 7‑digit */

    /* Sieve of Eratosthenes. */
    bool *is_prime = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!is_prime) return;   /* allocation failure: leave cache empty */

    for (uint32_t i = 2; i <= LIMIT; ++i) is_prime[i] = true;

    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* Collect primes with 7 distinct digits. */
    size_t cap = 8192;
    g_primes = (uint32_t*)malloc(cap * sizeof(uint32_t));
    if (!g_primes) { free(is_prime); return; }
    g_prime_cnt = 0;

    for (uint32_t n = 1000000U; n < LIMIT; ++n) {
        if (is_prime[n] && distinct_digits(n)) {
            if (g_prime_cnt == cap) {
                cap <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(g_primes, cap * sizeof(uint32_t));
                if (!tmp) break;   /* stop collecting if realloc fails */
                g_primes = tmp;
            }
            g_primes[g_prime_cnt++] = n;
        }
    }

    free(is_prime);

    /* No need to compute differences if there are fewer than two such primes. */
    if (g_prime_cnt < 2) {
        g_diff_cnt = 0;
        g_diffs = NULL;
        return;
    }

    /* Allocate an array of seq_t with proper construction using seq_alloc. */
    g_diff_cnt = g_prime_cnt - 1;
    g_diffs = seq_alloc(g_diff_cnt);
    if (!g_diffs) { g_diff_cnt = 0; return; }

    for (size_t i = 1; i < g_prime_cnt; ++i) {
        g_diffs[i - 1] = (seq_t)g_primes[i] - (seq_t)g_primes[i - 1];
    }
}

/* Public function requested by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of range */
    build();                       /* ensure caches are ready */
    if (g_diff_cnt == 0) return 0;
    if ((idx_t)(n - 1) >= g_diff_cnt) return 0;
    return g_diffs[(size_t)(n - 1)];
}
