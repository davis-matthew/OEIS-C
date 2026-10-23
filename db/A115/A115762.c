/*
 * A115762  Numbers k such that the digits of k^3, reversed, include the
 *          digits of k as substring.
 * https://oeis.org/A115762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   append_candidate, big_pow, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115762: Numbers k such that the digits of k^3,
   reversed, contain the digits of k as a contiguous substring.
   The terms are generated in increasing order.
*/

#include <stddef.h>
#include <stdbool.h>

#include "../utils/big_pow.h"      /* big_pow(seq_t base, seq_t exp) */
#include "../utils/seq_to_str.h"   /* seq_to_str(seq_t v, char *buf, size_t *len) */
#include "../utils/append_candidate.h" /* append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val) */

static seq_t *terms      = NULL;   /* array of found terms */
static size_t terms_size = 0;      /* number of terms stored */
static size_t terms_cap  = 0;      /* current allocation */
static idx_t next_k      = 1;      /* next integer to test */

/* Test whether k satisfies the condition. */
static bool matches_reversed_cube(seq_t k)
{
    seq_t cube = big_pow(k, 3);

    /* Get decimal representation of cube. */
    char cubebuf[512];
    size_t cubelen;
    seq_to_str(cube, cubebuf, &cubelen);
    cubebuf[cubelen] = '\0';

    /* Build reversed string of cube digits. */
    char revbuf[512];
    for (size_t i = 0; i < cubelen; ++i)
        revbuf[i] = cubebuf[cubelen - 1 - i];
    revbuf[cubelen] = '\0';

    /* Get decimal string of k. */
    char kbuf[128];
    size_t klen;
    seq_to_str(k, kbuf, &klen);
    kbuf[klen] = '\0';

    /* Simple substring check. */
    if (klen == 0) return true;
    for (size_t i = 0; i + klen <= cubelen; ++i) {
        size_t j;
        for (j = 0; j < klen; ++j)
            if (revbuf[i + j] != kbuf[j]) break;
        if (j == klen) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; undefined for n<1 */

    while (terms_size < n) {
        if (matches_reversed_cube(next_k))
            append_candidate(&terms, &terms_size, &terms_cap, next_k);
        ++next_k;
    }
    return terms[n - 1];
}
