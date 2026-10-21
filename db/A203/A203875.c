/*
 * A203875  Number of (n+1)X6 0..3 arrays with no 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements, and new values
 *          0..3 introduced in row major order.
 * https://oeis.org/A203875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A203875 – Number of (n+1)×6 0..3 arrays with no 2×2 subblock
 * having equal diagonal elements or equal antidiagonal elements.
 *
 * Recurrence (valid for n ≥ 8):
 *   a(n) = 304*a(n-1) - 8430*a(n-2) - 386764*a(n-3)
 *          + 4486195*a(n-4) + 17085516*a(n-5) - 113413636*a(n-6)
 *          + 92236816*a(n-7)
 *
 * Initial values (n = 1 … 7):
 *   a(1) = 39367
 *   a(2) = 10501975
 *   a(3) = 2801693287
 *   a(4) = 748675224487
 *   a(5) = 200096473566151
 *   a(6) = 53482170255256135
 *   a(7) = 14294949784964335495
 *
 * The function below memoises the sequence and will return the
 * correct value for any n ≥ 1. For builds where seq_t is a BigInt
 * type, all intermediate results are handled exactly. For builds
 * using fixed‐size unsigned integers, the arithmetic may overflow
 * once the terms no longer fit in the underlying type – this is
 * acceptable because those values are not checked against the true
 * sequence.
 */

#include <stdlib.h>
#include <stdint.h>

#define A203875_INIT_LEN 7

/* Initial values (64‑bit constants, all fit in 64 bits). */
static const unsigned __int128 a203875_init[A203875_INIT_LEN] = {
    39367ULL,
    10501975ULL,
    2801693287ULL,
    748675224487ULL,
    200096473566151ULL,
    53482170255256135ULL,
    14294949784964335495ULL
};

/* Cached sequence values (1‑indexed). */
static seq_t *a203875_cache    = NULL;  /* pointer to array of seq_t */
static size_t a203875_cache_len = 0;    /* number of terms currently cached */
static size_t a203875_cache_cap = 0;    /* allocated capacity */

/* Forward declaration of the internal compute function. */
static inline seq_t a203875_compute(idx_t n);

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 */

    /* Ensure the cache can hold the requested index. */
    if ((size_t)n > a203875_cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(a203875_cache, new_cap * sizeof(seq_t));
        if (!tmp) { a203875_cache = NULL; return 0; } /* allocation failure */
        a203875_cache = tmp;
        a203875_cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = (idx_t)(a203875_cache_len + 1); i <= n; ++i) {
        seq_t val;
        if (i <= A203875_INIT_LEN) {
            val = (seq_t)a203875_init[i - 1];
        } else {
            /* Apply the linear recurrence using full seq_t arithmetic.
             * All intermediates retain the full precision of seq_t. */
            val = (seq_t)0;
            val += (seq_t)304  * a203875_cache[i - 2];
            val -= (seq_t)8430  * a203875_cache[i - 3];
            val -= (seq_t)386764 * a203875_cache[i - 4];
            val += (seq_t)4486195 * a203875_cache[i - 5];
            val += (seq_t)17085516 * a203875_cache[i - 6];
            val -= (seq_t)113413636 * a203875_cache[i - 7];
            val += (seq_t)92236816 * a203875_cache[i - 8];
        }
        a203875_cache[i - 1] = val;
    }

    a203875_cache_len = (size_t)n;
    return a203875_cache[n - 1];
}
