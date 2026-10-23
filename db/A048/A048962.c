/*
 * A048962  Table in which n-th row lists digits in periodic part of decimal
 *          expansion of 1/n.
 * https://oeis.org/A048962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, tabf, nice
 * Formula:   not stated
 * Range:     u8 n <= 1192, u16 n <= 1192, u32 n <= 1192, u64 n <= 1192,
 *            u128 n <= 1192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A048962:  n‑th digit of the periodic part of 1/n in base 10.
 *  The function builds the table row by row, caching rows and their
 *  cumulative lengths.  Each row consists of the decimal digits that
 *  repeat in the expansion of 1/k.  For terminating decimals the
 *  period is the single digit 0.
 *
 *  The implementation uses only integer arithmetic, standard C11
 *  facilities, and is compatible with C++17.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*  Types supplied by the build system */
extern seq_t a(idx_t n);

/*  Data structure for a single row of the triangle   */
typedef struct {
    unsigned char *digits;   /* one digit per element   */
    size_t          len;     /* number of digits in row */
} Row;

/*  Cached rows and cumulative lengths   */
static Row *rows      = NULL;
static size_t rows_size = 0, rows_cap = 0;

static idx_t *cum      = NULL;
static size_t cum_size = 0, cum_cap = 0;

/*  Allocate or grow an array of Rows to a new capacity.   */
static void ensure_rows_cap(size_t new_cap)
{
    if (new_cap <= rows_cap) return;
    Row *tmp = (Row *)realloc(rows, new_cap * sizeof(Row));
    if (!tmp) abort();          /* out of memory – terminate */
    rows = tmp;
    rows_cap = new_cap;
}

/*  Allocate or grow an array of cumulative sums to a new capacity.   */
static void ensure_cum_cap(size_t new_cap)
{
    if (new_cap <= cum_cap) return;
    idx_t *tmp = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    cum = tmp;
    cum_cap = new_cap;
}

/*  Return the period of 1/k;  out_digits must be freed by the caller.   */
static void compute_period(idx_t k, unsigned char **out_digits, size_t *out_len)
{
    /*  For k = 1 the period is the single digit 0.   */
    if (k == 1) {
        *out_digits = (unsigned char *)malloc(1);
        (*out_digits)[0] = 0;
        *out_len = 1;
        return;
    }

    /*  Terminating decimals: k has no prime factor other than 2 or 5.   */
    idx_t tmp = k;
    while ((tmp % 2) == 0) tmp /= 2;
    while ((tmp % 5) == 0) tmp /= 5;
    if (tmp == 1) {            /* terminating */
        *out_digits = (unsigned char *)malloc(1);
        (*out_digits)[0] = 0;
        *out_len = 1;
        return;
    }

    /*  Non‑terminating: find the repeating block using remainders.   */
    size_t mod = (size_t)k;
    int *seen = (int *)malloc(mod * sizeof(int));
    if (!seen) abort();
    /*  Initialise to -1 .   */
    for (size_t i = 0; i < mod; ++i) seen[i] = -1;

    size_t cap = 16;
    unsigned char *digits = (unsigned char *)malloc(cap);
    if (!digits) abort();
    size_t sz = 0;

    idx_t remainder = 1 % k;   /* remainder before the decimal point   */
    while (1) {
        int pos = seen[remainder];
        if (pos != -1) { /* cycle found */
            size_t period_start = (size_t)pos;
            size_t period_len = sz - period_start;
            unsigned char *period = (unsigned char *)malloc(period_len);
            memcpy(period, digits + period_start, period_len);
            free(seen);
            free(digits);
            *out_digits = period;
            *out_len   = period_len;
            return;
        }
        seen[remainder] = (int)sz;
        unsigned long long tmp = (unsigned long long)remainder * 10ULL;
        unsigned char digit = (unsigned char)(tmp / k);
        remainder = (idx_t)(tmp % k);
        if (sz == cap) {
            cap <<= 1;
            unsigned char *tmp2 = (unsigned char *)realloc(digits, cap);
            if (!tmp2) abort();
            digits = tmp2;
        }
        digits[sz++] = digit;
    }
}

/*  Main sequence function   */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /*  offset is 1, but guard against 0  */

    /*  Grow the cache until it contains the requested index.   */
    while (cum_size == 0 || cum[cum_size - 1] < n) {
        idx_t next_k = (idx_t)(rows_size + 1);   /* k of the next row   */

        /*  Compute period digits for 1/next_k.   */
        unsigned char *digits;
        size_t len;
        compute_period(next_k, &digits, &len);

        /*  Store the new row.   */
        ensure_rows_cap(rows_size + 1);
        rows[rows_size].digits = digits;
        rows[rows_size].len    = len;
        ++rows_size;

        /*  Update cumulative length.   */
        idx_t new_cum = (cum_size == 0) ?
            (idx_t)len : cum[cum_size - 1] + (idx_t)len;
        ensure_cum_cap(cum_size + 1);
        cum[cum_size] = new_cum;
        ++cum_size;
    }

    /*  Find the row that contains the n‑th digit.   */
    size_t row_index = 0;
    while (row_index < cum_size && cum[row_index] < n) ++row_index;

    idx_t offset = (row_index == 0) ? 0 : cum[row_index - 1];
    size_t pos   = (size_t)(n - offset - 1);  /* zero‑based position   */

    return (seq_t)rows[row_index].digits[pos];
}
