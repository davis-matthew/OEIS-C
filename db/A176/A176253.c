/*
 * A176253  Indices n corresponding to Fibonacci numbers Fib(n) whose decimal
 *          expansion does not contain the digit "5".
 * https://oeis.org/A176253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A176253: a(n) = index of the nth Fibonacci number whose
 * decimal expansion does NOT contain the digit '5'.  The implementation
 * uses an arbitrary‑precision decimal representation on the fly, so it works
 * for all builds (uint64_t or big‑int). */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* a and b are arrays of decimal digits (LSB first), len_* are lengths.
 * The result is stored in *out, which may be realloc'd.              */
static void big_add(const uint8_t *a, size_t la,
                    const uint8_t *b, size_t lb,
                    uint8_t **out, size_t *len_out)
{
    size_t max = la > lb ? la : lb;
    size_t new_cap = max + 1;
    uint8_t *res = (uint8_t *)realloc(*out, new_cap);
    if (!res) abort();
    uint32_t carry = 0;
    size_t i;
    for (i = 0; i < max; ++i) {
        uint32_t da = (i < la) ? a[i] : 0;
        uint32_t db = (i < lb) ? b[i] : 0;
        uint32_t sum = da + db + carry;
        res[i] = sum % 10;
        carry  = sum / 10;
    }
    if (carry) {
        res[i++] = carry;
    }
    *len_out = i;
    *out     = res;
}

/* check if a decimal number (digits LSB first) contains the digit 5 ----- */
static bool has_no_five(const uint8_t *digits, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        if (digits[i] == 5) return false;
    }
    return true;
}

/* static state ========================================================== */

/* cache of indices that satisfy the condition */
static idx_t *idx_cache = NULL;
static size_t idx_cap   = 0;
static size_t idx_cnt   = 0;

/* current Fibonacci numbers in decimal, least‑significant digit first */
static uint8_t *fib_prev = NULL;   /* F(k-1) */
static uint8_t *fib_curr = NULL;   /* F(k)   */
static size_t   len_prev = 0;
static size_t   len_curr = 0;
static idx_t    curr_k   = 1;       /* index of fib_curr */
static bool     initialized = false;

/* ---------------------------------------------------------------------- */
/* helper to grow the integer cache ------------------------------------- */
static void grow_idx_cache(void)
{
    size_t new_cap = idx_cap ? idx_cap * 2 : 64;
    idx_t *tmp = (idx_t *)realloc(idx_cache, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    idx_cache = tmp;
    idx_cap = new_cap;
}

/* ---------------------------------------------------------------------- */
/* arbitrary‑precision addition of two decimal numbers ------------------- */

/* ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
/* initialize static state if needed ------------------------------------ */
static void init_state(void)
{
    if (initialized) return;
    initialized = true;

    /* allocate cache */
    idx_cap   = 64;
    idx_cache = (idx_t *)malloc(idx_cap * sizeof(idx_t));
    if (!idx_cache) abort();

    /* initialise Fibonacci numbers: 0 and 1 */
    fib_prev = (uint8_t *)malloc(sizeof(uint8_t));
    fib_curr = (uint8_t *)malloc(sizeof(uint8_t));
    if (!fib_prev || !fib_curr) abort();
    fib_prev[0] = 0; len_prev = 1;
    fib_curr[0] = 1; len_curr = 1;
    curr_k = 1;      /* fib_curr index */
}

/* ---------------------------------------------------------------------- */
/* main function required by the framework -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1, safe fallback */

    init_state();

    /* grow the list of qualifying indices until we have at least n of them */
    while (idx_cnt < (size_t)n) {
        if (has_no_five(fib_curr, len_curr)) {
            if (idx_cnt == idx_cap) grow_idx_cache();
            idx_cache[idx_cnt++] = curr_k;
        }

        /* compute next Fibonacci number */
        uint8_t *next = NULL;
        size_t  next_len = 0;
        big_add(fib_prev, len_prev, fib_curr, len_curr, &next, &next_len);

        /* shift values: fib_prev <- fib_curr, fib_curr <- next */
        free(fib_prev);
        fib_prev = fib_curr; len_prev = len_curr;
        fib_curr = next;   len_curr = next_len;

        curr_k++;                 /* index of new fib_curr */
    }

    return (seq_t)idx_cache[n - 1];
}
