/*
 * A213175  Primes p such that subtracting two from any single decimal digit
 *          of p produces another prime with restriction that first digit may
 *          not be changed to a 0 (obviously p cannot contain digits less than
 *          2).
 * https://oeis.org/A213175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 10, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A213175 – primes such that subtracting two from any single decimal digit yields
 * another prime, with the restriction that the leading digit may not become 0
 * (hence p contains only digits 2–9 and the first digit is at least 3). The
 * function returns the n‑th term (n ≥ 1) and caches results so that subsequent
 * calls are fast.  */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/pow10.h"          /* seq_t pow10(idx_t e); */

/* ------------------------------------------------------------------------ */
/*                     local helper: digits of a number                      */
/* ------------------------------------------------------------------------ */
static void digits_of(seq_t n, uint8_t *buf, size_t *len)
{
    if (n == (seq_t)0) {
        *len = 1;
        buf[0] = 0;
        return;
    }
    size_t pos = 0;
    seq_t tmp = n;
    uint8_t rev[128];
    while (tmp != (seq_t)0) {
        rev[pos++] = (uint8_t)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }
    /* reverse to most‑significant first */
    for (size_t i = 0; i < pos; ++i)
        buf[i] = rev[pos - 1 - i];
    *len = pos;
}

/* ------------------------------------------------------------------------ */
/*                dynamic array of known terms (cached)                     */
/* ------------------------------------------------------------------------ */
static seq_t *seq_terms   = NULL;
static size_t term_cnt    = 0;
static size_t term_cap    = 0;
static seq_t  last_seen   = (seq_t)4;      /* prime 5 will be first examined */

static void ensure_cap(size_t needed)
{
    if (needed <= term_cap)
        return;
    size_t new_cap = term_cap ? term_cap : 128;
    while (new_cap < needed)
        new_cap <<= 1;
#if defined(__cplusplus)
    seq_t *new_ptr = new seq_t[new_cap];
    if (seq_terms && term_cnt)
        memcpy(new_ptr, seq_terms, term_cnt * sizeof(seq_t));
    delete[] seq_terms;
    seq_terms = new_ptr;
    term_cap = new_cap;
#else
    seq_t *new_ptr = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();                          /* memory error – unlikely */
    seq_terms = new_ptr;
    term_cap = new_cap;
#endif
}

/* ------------------------------------------------------------------------ */
/*                         public sequence function                         */
/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)            /* behaviour below offset is irrelevant */
        return (seq_t)0;

    /* generate terms until we have at least n of them */
    while (term_cnt < (size_t)n) {
        /* search for next candidate */
        seq_t cand;
        uint8_t d[128];
        size_t len;
        bool ok;

    next_candidate:
        ++last_seen;
        cand = last_seen;
        digits_of(cand, d, &len);

        /* digits must all be >= 2 and leading digit >= 3 */
        ok = true;
        for (size_t i = 0; i < len; ++i) {
            if (d[i] < 2 || (i == 0 && d[i] == 2)) {
                ok = false;
                break;
            }
        }
        if (!ok)
            goto next_candidate;

        /* candidate must be prime */
        if (!is_prime_big(cand))
            goto next_candidate;

        /* all single‑digit decrements must also be prime */
        for (size_t i = 0; i < len && ok; ++i) {
            seq_t y = cand - (seq_t)2 * pow10((idx_t)(len - 1 - i));
            if (!is_prime_big(y))
                ok = false;
        }

        if (!ok)
            goto next_candidate;

        /* cand satisfies the property → store it */
        ensure_cap(term_cnt + 1);
        seq_terms[term_cnt++] = cand;
    }

    return seq_terms[n - 1];
}
