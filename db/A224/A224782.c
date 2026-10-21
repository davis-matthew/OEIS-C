/*
 * A224782  Length of longest run of consecutive zeros in decimal
 *          representation of 2^n.
 * https://oeis.org/A224782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes longest run of consecutive zeros in the decimal representation
 * of 2^n (offset 0).  a(n) is the length of that run for a given n. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Static state ------------------------------------------------------------*/
static idx_t      max_n     = 0;          /* largest n cached in a_cache   */
static seq_t     *a_cache   = NULL;       /* cache of a(0)..a(max_n)       */
static size_t    cache_cap  = 0;          /* allocated size of a_cache     */

static uint8_t   *digits    = NULL;       /* decimal digits of 2^max_n (little‑endian) */
static size_t    digits_len = 0;          /* number of used digits           */
static size_t    digits_cap = 0;          /* allocated digits capacity       */

/* Ensure a_cache can hold at least 'need' entries --------------------------*/
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1U : 16;
    while (new_cap < need) new_cap <<= 1U;
#if defined(__cplusplus)
    seq_t *tmp = static_cast<seq_t*>(realloc(a_cache, new_cap * sizeof(seq_t)));
#else
    seq_t *tmp = realloc(a_cache, new_cap * sizeof(seq_t));
#endif
    if (!tmp && new_cap > 0) exit(1);     /* allocation failed */
    a_cache   = tmp;
    cache_cap = new_cap;
}

/* Ensure digits array can hold at least 'need' slots -----------------------*/
static void ensure_digits(size_t need)
{
    if (need <= digits_cap) return;
    size_t new_cap = digits_cap ? digits_cap << 1U : 16;
    while (new_cap < need) new_cap <<= 1U;
#if defined(__cplusplus)
    uint8_t *tmp = static_cast<uint8_t*>(realloc(digits, new_cap));
#else
    uint8_t *tmp = realloc(digits, new_cap);
#endif
    if (!tmp && new_cap > 0) exit(1);
    digits    = tmp;
    digits_cap = new_cap;
}

/* Multiply current 2^max_n by 2 and compute the longest zero run length -----*/
static seq_t mul2_and_find_run(void)
{
    size_t i;
    uint8_t carry = 0;
    for (i = 0; i < digits_len; ++i) {
        uint8_t t = digits[i] * 2 + carry;
        digits[i] = t % 10;
        carry = t / 10;
    }
    if (carry) {
        ensure_digits(digits_len + 1);
        digits[digits_len++] = carry;
    }

    seq_t run = 0, max_run = 0;
    for (i = 0; i < digits_len; ++i) {
        if (digits[i] == 0) {
            ++run;
        } else {
            if (run > max_run) max_run = run;
            run = 0;
        }
    }
    if (run > max_run) max_run = run;
    return max_run;
}

/* --------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (digits == NULL) {
#if defined(__cplusplus)
        digits = static_cast<uint8_t*>(malloc(1));
#else
        digits = malloc(1);
#endif
        if (!digits) return 0;         /* out of memory */
        digits[0] = 1;                 /* 2^0 = 1 */
        digits_len = 1;
        digits_cap = 1;
        ensure_cache(n + 1);
        a_cache[0] = 0;                /* "1" has no zero run */
        max_n = 0;
    }

    /* extend cache up to requested n if necessary */
    if (n > max_n) {
        for (idx_t k = max_n + 1; k <= n; ++k) {
            seq_t run = mul2_and_find_run();
            ensure_cache(k + 1);
            a_cache[k] = run;
        }
        max_n = n;
    }

    return a_cache[n];
}
