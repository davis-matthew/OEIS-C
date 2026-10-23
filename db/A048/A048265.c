/*
 * A048265  Positive integers that aren't the sum of distinct primes of the
 *          form 6k+1.
 * https://oeis.org/A048265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th positive integer that cannot be expressed
   as the sum of distinct primes of the form 6k+1.  The sequence is
   finite; the largest term is 205. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define MAX_LIMIT 205          /* maximum value to consider (largest term is 205) */
#define PRIME_COUNT 50        /* sufficient to hold all 6k+1 primes up to 205 */

/* Simple primality test for small numbers (used for primes ≤ MAX_LIMIT). */
static bool is_prime_small(idx_t n)
{
    if (n < 2) return false;
    if ((n & 1) == 0) return n == 2;
    for (idx_t d = 3; d*d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Global data: list of non‑representable numbers and its length. */
static seq_t non_rep_list[256];
static size_t non_rep_len = 0;

/* One‑time initialization: build the list of primes 6k+1,
   compute representable sums by subset sum DP,
   and collect the non‑representable numbers. */
static void init_non_rep(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* Collect primes of the form 6k+1 up to MAX_LIMIT. */
    idx_t primes[PRIME_COUNT];
    size_t prime_cnt = 0;
    for (idx_t p = 2; p <= MAX_LIMIT; ++p)
        if (is_prime_small(p) && (p % 6 == 1))
            primes[prime_cnt++] = p;

    /* Subset–sum DP to mark representable numbers. */
    bool dp[MAX_LIMIT + 1];
    for (size_t i = 0; i <= MAX_LIMIT; ++i) dp[i] = false;
    dp[0] = true;   /* empty sum */

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];
        for (idx_t x = MAX_LIMIT; x >= p; --x)
            if (dp[x - p]) dp[x] = true;
    }

    /* Gather all positive integers up to MAX_LIMIT that are NOT representable. */
    non_rep_len = 0;
    for (idx_t x = 1; x <= MAX_LIMIT; ++x)
        if (!dp[x]) {
            non_rep_list[non_rep_len++] = (seq_t)x;
            if (non_rep_len >= sizeof(non_rep_list)/sizeof(non_rep_list[0]))
                break;   /* safety guard (should not happen) */
        }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    init_non_rep();                           /* ensure list is ready */
    if (n == 0 || n > non_rep_len) return (seq_t)0;
    return non_rep_list[n - 1];
}
