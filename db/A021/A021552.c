/*
 * A021552  Decimal expansion of 1/548.
 * https://oeis.org/A021552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal expansion of 1/548 – each a(n) is the n‑th digit after the decimal point.
   The long division algorithm is used:
       r_0 = 1
       a(k) = floor(r_k * 10 / 548)
       r_{k+1} = r_k * 10 mod 548
   The function caches digits for repeated calls. */
#include <stdlib.h>

static seq_t *digits = NULL;      /* cached digits */
static size_t digits_len = 0;     /* number of digits stored */
static size_t digits_cap = 0;     /* allocated capacity */
static seq_t remainder = (seq_t)1;/* remainder after the last computed digit */

/* Ensure the cache can hold at least `need` entries. */
static void grow_cache(size_t need)
{
    if (digits_cap >= need)
        return;
    size_t new_cap = digits_cap ? digits_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_arr = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
    if (!new_arr) {
        /* Allocation failed – terminate the program safely. */
        abort();
    }
    digits = new_arr;
    digits_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Convert idx_t to size_t for array indexing – it is non‑negative. */
    size_t idx = (size_t)n;

    /* Compute digits up to the requested index. */
    while (digits_len <= idx) {
        /* long division step */
        seq_t t = remainder * (seq_t)10;
        seq_t d = t / (seq_t)548;
        remainder = t % (seq_t)548;

        /* Cache the digit. */
        grow_cache(digits_len + 1);
        digits[digits_len] = d;
        digits_len++;
    }

    return digits[idx];
}
