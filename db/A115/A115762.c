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
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115762: Numbers k such that the digits of k^3,
   reversed, contain the digits of k as a contiguous substring.
   The terms are generated in increasing order.
*/

#include <stddef.h>
#include <stdbool.h>

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static seq_t *terms      = NULL;   /* array of found terms */
static size_t terms_size = 0;      /* number of terms stored */
static size_t terms_cap  = 0;      /* current allocation */
static idx_t next_k      = 1;      /* next integer to test */

/* Test whether k satisfies the condition. */
static bool matches_reversed_cube(seq_t k)
{
    seq_t cube = ipow(k, 3);

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
