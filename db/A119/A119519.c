/*
 * A119519  The first 10 digits of the fourth root of n contain the digits
 *          0-9.
 * https://oeis.org/A119519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): the nth positive integer for which the first 10 digits of the fourth
   root of n contain all digits 0‑9.  A search with memoization is used. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/*--- utility ----------------------------------------------------*/

static inline int digits_of(uint64_t x) {
    int d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

/* test whether the first 10 digits of n^(1/4) contain each decimal digit
   exactly once (ignoring the decimal point). */
static inline bool has_all_digits(idx_t n) {
    double r = pow((double)n, 0.25);          /* fourth root */

    uint64_t intpart = (uint64_t)r;           /* integer part */
    int dk = digits_of(intpart);              /* digits before decimal point */

    /* scale so that the first 10 digits become the integer part */
    double scaled = r * pow(10.0, 10 - dk);
    uint64_t first10 = (uint64_t)(scaled + 1e-7);  /* guard against rounding */

    unsigned char cnt[10] = {0};
    for (int i = 0; i < 10; ++i) {
        cnt[first10 % 10] += 1;
        first10 /= 10;
    }

    for (int i = 0; i < 10; ++i)
        if (cnt[i] != 1)
            return false;
    return true;
}

/*--- cache --------------------------------------------------------*/

static seq_t *seq_cache = NULL;
static size_t  seq_cap   = 0;   /* current capacity of seq_cache */
static idx_t   seq_len   = 0;   /* number of terms stored */
static idx_t   last_checked = 0;   /* last n examined */

/* ensure the cache can hold at least 'need' elements */
static void ensure_cache_capacity(size_t need) {
    if (seq_cap >= need)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();              /* allocation failure */
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/*--- main function ------------------------------------------------*/

seq_t a(idx_t n) {
    if (n == 0)            /* undefined at offset 0; sentinel value */
        return (seq_t)0;

    if (seq_len < n) {
        ensure_cache_capacity(n);
        idx_t cand = last_checked + 1;
        while (seq_len < n) {
            if (has_all_digits(cand))
                seq_cache[seq_len++] = (seq_t)cand;
            ++cand;
        }
        last_checked = cand - 1;
    }
    return seq_cache[n - 1];
}
