/*
 * A023372  a(1) = 3; for n>=1, a(n+1) = a(n) converted to base 10 from base 3
 *          (written in base 10).
 * https://oeis.org/A023372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3; for n≥1, a(n+1) is the decimal interpretation of the base‑3 digits of a(n). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;   /* memoized sequence values              */
static size_t  cap   = 0;      /* current cache capacity                */
static idx_t   len   = 0;      /* number of terms already computed      */

static uint8_t *digits = NULL;/* temporary buffer for base‑3 digits   */
static size_t  dcap  = 0;     /* capacity of the digit buffer          */

/* Ensure cache holds at least N elements. */
static void ensure_cache_capacity(idx_t N)
{
    if (cap >= (size_t)N) return;
    size_t new_cap = cap ? cap << 1U : 8U;
    while (new_cap < (size_t)N) new_cap <<= 1U;

    if (cache == NULL) {
#ifdef __cplusplus
        cache = new seq_t[new_cap];
#else
        cache = (seq_t*)malloc(sizeof(seq_t) * new_cap);
#endif
    } else {
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
        delete [] cache;
        cache = tmp;
#else
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
#endif
    }
    cap = new_cap;
}

/* Ensure digit buffer holds at least M digits. */
static void ensure_digits_capacity(size_t M)
{
    if (dcap >= M) return;
    size_t new_cap = dcap ? dcap << 1U : 16U;
    while (new_cap < M) new_cap <<= 1U;
    uint8_t *tmp = (uint8_t*)realloc(digits, new_cap);
    digits = tmp;
    dcap = new_cap;
}

/* Compute the next term from the current value. */
static seq_t next_term(seq_t val)
{
    /* Extract base‑3 digits of val (least significant first). */
    size_t dlen = 0;
    if (val == (seq_t)0) {          /* should not happen, but keep safe */
        dlen = 1;
        ensure_digits_capacity(1);
        digits[0] = 0;
    } else {
        seq_t tmp = val;
        while (tmp != (seq_t)0) {
            ensure_digits_capacity(dlen + 1);
            digits[dlen++] = (uint8_t)(tmp % (seq_t)3);
            tmp = tmp / (seq_t)3;
        }
    }

    /* Interpret the digits as a base‑10 number. */
    seq_t next = (seq_t)0;
    for (size_t i = 0; i < dlen; ++i) {
        next = next * (seq_t)10 + (seq_t)digits[dlen - 1 - i];
    }
    return next;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;      /* undefined, but safe */

    if (len == (idx_t)0) {                   /* first call: initialise a(1)=3 */
        ensure_cache_capacity(1);
        cache[0] = (seq_t)3;
        len = (idx_t)1;
    }

    if (len < n) {
        ensure_cache_capacity(n);
        for (idx_t i = len; i < n; ++i) {
            cache[i] = next_term(cache[i - 1]);
        }
        len = n;
    }
    return cache[n - 1];
}
