/*
 * A107322  English name for number and its reverse have the same number of
 *          letters.
 * https://oeis.org/A107322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A107322 – English name for number and its reverse have the same
    number of letters.  Computation is incremental search: numbers are
    examined in increasing order; when a number satisfies the condition
    its value is stored in a dynamic array.  The English word
    length is calculated recursively following the standard
    English‑number naming rules (hyphens and spaces omitted).  The
    thousands part is handled up to 999 999.  */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/reverse_uint.h"

static const seq_t unit_len[10] = {
    4, /* zero   */
    3, /* one    */
    3, /* two    */
    5, /* three  */
    4, /* four   */
    4, /* five   */
    3, /* six    */
    5, /* seven  */
    5, /* eight  */
    4  /* nine   */
};

static const seq_t teen_len[10] = {
    3, /* ten     */
    6, /* eleven  */
    6, /* twelve  */
    8, /* thirteen*/
    8, /* fourteen*/
    7, /* fifteen */
    7, /* sixteen */
    9, /* seventeen*/
    8, /* eighteen */
    8  /* nineteen */
};

static const seq_t tens_len[8] = {
    6, /* twenty  */
    6, /* thirty  */
    5, /* forty   */
    5, /* fifty   */
    5, /* sixty   */
    7, /* seventy */
    6, /* eighty  */
    6  /* ninety  */
};

/* compute the number of letters in the English name of n
   (hyphens and spaces are omitted). */
static seq_t english_len(uint64_t n)
{
    if (n == 0) return unit_len[0];            /* zero */

    seq_t len = 0;

    /* thousands (handle up to 999,999 correctly) */
    if (n >= 1000) {
        uint64_t thousands = n / 1000;
        len += english_len(thousands);          /* recursive part */
        len += 8;                               /* "thousand" */
        n %= 1000;
        if (n == 0) return (seq_t)len;
    }

    /* hundreds */
    if (n >= 100) {
        uint64_t hundreds = n / 100;
        len += unit_len[hundreds];
        len += 7;                               /* "hundred" */
        n %= 100;
        if (n == 0) return (seq_t)len;
    }

    /* tens and units */
    if (n >= 20) {
        uint64_t tens = n / 10;
        len += tens_len[tens - 2];              /* tens_len[0] = 20 */
        n %= 10;
    } else if (n >= 10) {
        len += teen_len[n - 10];
        return (seq_t)len;
    }

    if (n > 0) {
        len += unit_len[n];
    }
    return (seq_t)len;
}

/* dynamic array to store sequence terms */
static seq_t *seq_arr = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;        /* number of terms stored */
static uint64_t next_candidate = 1;   /* next number to test */

static void seq_ensure_capacity(void)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Initialise the sequence on the first call. */
    if (seq_len == 0) {
        seq_cap = 8;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_arr[0] = 0;        /* a(0) = 0 */
        seq_len = 1;
        next_candidate = 1;    /* start searching from 1 */
    }

    /* Extend the sequence until we have at least n+1 terms. */
    while (seq_len <= (size_t)n) {
        uint64_t cand = next_candidate++;
        seq_t len_cand = english_len(cand);
        uint64_t rev = reverse_uint(cand);
        seq_t len_rev = english_len(rev);
        if (len_cand == len_rev) {
            seq_ensure_capacity();
            seq_arr[seq_len++] = (seq_t)cand;
        }
    }

    return seq_arr[n];
}
