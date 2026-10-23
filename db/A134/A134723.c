/*
 * A134723  Concatenation of next n odd primes.
 * https://oeis.org/A134723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 5, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = concatenation of the next n odd primes, beginning with the
 * (1 + n(n−1)/2)-th odd prime. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/pow10.h"
#include "../utils/digit_count.h"

/* Cached prime list, including 2 as the first element. */
static uint64_t *prime_arr = NULL;
static size_t   prime_cnt  = 0;   /* total number of primes in prime_arr */

/* Ensure at least `need_odd` odd primes are available.  The helper
 * _prime_limit_for_idx counts all primes, so we ask for need_odd+1
 * primes overall (including the even prime 2). */
static void
ensure_primes_for_index(idx_t need_odd)
{
    /* we need at least need_odd odd primes → need_odd+1 total primes. */
    if (prime_cnt > (size_t)need_odd)
        return;   /* already enough */

    idx_t limit = _prime_limit_for_idx(need_odd + 1);
    uint64_t *tmp = NULL;
    size_t tmp_cnt = 0;
    build_primes_upto(limit, &tmp, &tmp_cnt);    /* primes ≤ limit, sorted */
    free(prime_arr);
    prime_arr = tmp;
    prime_cnt = tmp_cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    /* Number of odd primes needed up to the end of the block:
       the end 1‑based odd‑prime index = n(n+1)/2. */
    idx_t need_odd = n * (n + 1) / 2;
    ensure_primes_for_index(need_odd);

    /* Starting odd‑prime index (1‑based). */
    idx_t start_idx = n * (n - 1) / 2 + 1;

    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i) {
        /* Map the 1‑based odd‑prime index to the array index.
           prime_arr[0] = 2, prime_arr[1] = 3, ... */
        size_t aidx = (size_t)(start_idx + i);
        uint64_t p = prime_arr[aidx];          /* odd prime value */
        seq_t  p_val = (seq_t)p;
        seq_t  powv   = pow10((idx_t)digit_count(p));
        result = result * powv + p_val;
    }
    return result;
}
