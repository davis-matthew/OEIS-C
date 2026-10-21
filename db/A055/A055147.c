/*
 * A055147  The first n digits of the juxtaposition of the base-6 numbers
 *          converted to decimal.
 * https://oeis.org/A055147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the decimal value of the number whose base‑6
 *            representation is the first n digits of the concatenation of the
 *            base‑6 representations of the integers 1,2,3,
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static cache of previously computed terms */
static seq_t  *a_cache   = NULL;
static idx_t   a_cache_len = 0;      /* number of values stored */
static size_t  a_cache_cap = 0;      /* allocated capacity */

/* State variables for generating the stream of base‑6 digits */
static idx_t           next_k      = 1;      /* next integer to output */
static unsigned char   cur_digits[40]; /* digits of current integer in base‑6 */
static size_t          cur_digit_len = 0;    /* length of cur_digits  */
static size_t          cur_digit_pos = 0;    /* next digit to use */

/* Helper: ensure that the cache can store up to at least n elements */
static void ensure_cache_capacity(idx_t n)
{
    if (n <= a_cache_len) return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 1;
    while (new_cap < (size_t)n) new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) { /* allocation failure */
        exit(EXIT_FAILURE);
    }
    if (a_cache) {
        memcpy(new_cache, a_cache, (size_t)a_cache_len * sizeof(seq_t));
        free(a_cache);
    }
    a_cache = new_cache;
    a_cache_cap = new_cap;
}

/* Helper: produce the next base‑6 digit from the global stream */
static unsigned char next_digit(void)
{
    if (cur_digit_pos >= cur_digit_len) {
        /* Generate digits for the next integer */
        idx_t          tmp   = next_k;
        unsigned char  buf[40];
        size_t         len   = 0;
        do {
            buf[len++] = (unsigned char)(tmp % 6);
            tmp /= 6;
        } while (tmp != 0);
        /* Reverse into cur_digits (most significant first) */
        for (size_t i = 0; i < len; ++i)
            cur_digits[i] = buf[len - 1 - i];
        cur_digit_len  = len;
        cur_digit_pos  = 0;
        ++next_k;
    }
    return cur_digits[cur_digit_pos++];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset < 1, but return 0 */

    ensure_cache_capacity(n);

    /* Compute terms up to n */
    for (idx_t i = a_cache_len; i < n; ++i) {
        unsigned char d = next_digit();
        seq_t prev = (i == 0 ? (seq_t)0 : a_cache[i - 1]);
        a_cache[i] = prev * 6 + d;
    }
    a_cache_len = n;  /* update count */

    return a_cache[n - 1];
}
