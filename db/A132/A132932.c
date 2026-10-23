/*
 * A132932  Concatenation of first n composite numbers.
 * https://oeis.org/A132932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   concatenation of the first n composite numbers
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   digits, is_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a small cache of composite numbers and
   a cache of the sequence values to support arbitrary order of
   queries.  The concatenation of two numbers s and t is performed
   as s * 10^d(t) + t, where d(t) is the number of decimal digits
   of t.  For arbitrary-precision types the required helpers
   pow10 and digits are provided by the utils library. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/pow10.h"
#include "../utils/digits.h"

/* cache of composite numbers ------------------------------------------------*/
static uint64_t *composites   = NULL;
static size_t   comp_cap      = 0;
static size_t   comp_cnt      = 0;
static uint64_t next_test     = 4;          /* start testing at 4 */

/* cache of sequence values --------------------------------------------------*/
static seq_t *values         = NULL;
static size_t values_cap     = 0;
static size_t values_len     = 0;

/* ---------------------------------------------------------------------------*/
static void ensure_composites(size_t need)
{
    if (comp_cnt >= need)
        return;

    /* grow capacity */
    if (comp_cap == 0) comp_cap = 128;
    while (comp_cap < need)
        comp_cap <<= 1;
    composites = (uint64_t*)realloc(composites, comp_cap * sizeof(uint64_t));

    /* generate composites until we have `need` of them */
    while (comp_cnt < need) {
        if (next_test >= 4 && !is_prime(next_test))
            composites[comp_cnt++] = next_test;
        ++next_test;
    }
}

/* ---------------------------------------------------------------------------*/
static void ensure_values_size(size_t need)
{
    if (values_len >= need)
        return;

    if (values_cap == 0) values_cap = 128;
    while (values_cap < need)
        values_cap <<= 1;
    values = (seq_t*)realloc(values, values_cap * sizeof(seq_t));
}

/* ---------------------------------------------------------------------------*/
extern seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but safe for internal use */

    /* we need at least n composite numbers */
    ensure_composites((size_t)n);

    /* compute sequence values up to n if we haven't yet */
    if (values_len < (size_t)n) {
        ensure_values_size((size_t)n);
        for (size_t i = values_len; i < (size_t)n; ++i) {
            uint64_t comp = composites[i];
            if (i == 0) {
                values[i] = (seq_t)comp;                   /* first term is 4 */
            } else {
                int d = digits(comp);                      /* digits of current composite */
                seq_t extend = pow10(d);                   /* 10^d in seq_t */
                values[i] = values[i-1] * extend + (seq_t)comp;
            }
        }
        values_len = (size_t)n;
    }

    return values[n - 1];
}
