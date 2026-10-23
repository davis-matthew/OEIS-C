/*
 * A022515  Describe previous term from the right (method B - initial term is
 *          4).
 * https://oeis.org/A022515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(0) = 4; for n > 0, a(n) is obtained from a(n-1) by describing
 *            the decimal representation of a(n-1) from right to left: each
 *            maximal block of identical consecutive digits is replaced by the
 *            block digit followed by the block length
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Forward declaration of the helper that computes the next term. */
static seq_t next_term(seq_t prev);

/* Public function required by the evaluation system. */
seq_t a(idx_t n)
{
    /* Static memoisation array. */
    static seq_t *cache      = NULL;   /* terms 0 .. cache_len-1 */
    static idx_t  cache_len  = 0;      /* number of terms stored */
    static idx_t  cache_cap  = 0;      /* allocated capacity */

    /* Grow the capacity if necessary. */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap < n + 1) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – terminate. */
            exit(EXIT_FAILURE);
        }
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the cache with the first term. */
    if (cache_len == 0) {
        cache[0] = (seq_t)4;
        cache_len = 1;
    }

    /* Compute new terms up to n. */
    for (idx_t k = cache_len; k <= n; ++k) {
        cache[k] = next_term(cache[k - 1]);
    }
    cache_len = n + 1;
    return cache[n];
}

/* ------------------------------------------------------------------ */
/* Compute the next term of the sequence from the previous term. */
static seq_t next_term(seq_t prev)
{
    /* Handle the anomalous case prev == 0 (should not occur). */
    if (prev == 0) {
        return (seq_t)0;
    }

    /* Step 1: Extract decimal digits of prev into an array LSB‑first. */
    idx_t len = 0;
    seq_t temp = prev;
    while (temp != (seq_t)0) {
        temp /= (seq_t)10;
        ++len;
    }
    unsigned char *digits = (unsigned char *)malloc(len * sizeof(unsigned char));
    if (!digits) {
        exit(EXIT_FAILURE);
    }
    temp = prev;
    for (idx_t i = 0; i < len; ++i) {
        digits[i] = (unsigned char)(temp % (seq_t)10);
        temp /= (seq_t)10;
    }

    /* Step 2: Process groups from rightmost digit. */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < len; ) {
        unsigned char d = digits[i];
        idx_t cnt = 1;
        while (i + cnt < len && digits[i + cnt] == d) {
            ++cnt;
        }

        /* Append the digit itself. */
        result = result * (seq_t)10 + (seq_t)d;

        /* Encode the count in decimal, most significant digit first. */
        /* Temporary buffer for the digits of cnt. */
        unsigned char cnt_buf[32];
        idx_t cnt_len = 0;
        idx_t c = cnt;
        do {
            cnt_buf[cnt_len++] = (unsigned char)(c % (idx_t)10);
            c /= (idx_t)10;
        } while (c > 0);
        for (idx_t j = cnt_len; j > 0; --j) {
            result = result * (seq_t)10 + (seq_t)(cnt_buf[j - 1]);
        }

        i += cnt;
    }

    free(digits);
    return result;
}
