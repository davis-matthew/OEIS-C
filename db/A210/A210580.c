/*
 * A210580  Concatenation of 1, 2, 3, ... represented in base 3/2 in the
 *          spirit of Champernowne's constant.
 * https://oeis.org/A210580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = nth digit of the concatenation of base‑3/2
 *            representations of 1, 2, 3, ... (the Champernowne–type constant
 *            in base 3/2)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helpers for dynamic array growth ----------------------------------------- */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                         /* Out of memory, cannot recover   */
    *arr = tmp;
    *cap = new_cap;
}

/* Core routine ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static state retained between calls.  */
    static seq_t *digits = NULL;   /* array of base‑3/2 digits          */
    static size_t digits_cap = 0;  /* allocated capacity                */
    static size_t digits_len = 0;  /* number of digits currently stored */
    static idx_t next_integer = 1; /* next integer whose rep is needed   */

    /* Grow the array until we have at least n+1 digits. */
    while (digits_len <= n) {
        /* Generate the base‑3/2 representation of next_integer.  */
        idx_t t = next_integer << 1;     /* start with 2 * next_integer     */
        uint8_t tmp[64];                 /* digits are at most 64            */
        size_t tmp_len = 0;

        /* Extract digits least‑significant first. */
        while (t > 0) {
            uint8_t d = (uint8_t)(t % 3);          /* d ∈ {0,1,2}                */
            tmp[tmp_len++] = d;
            /* t' = (t – d)/3 * 2  (exact division by 3 guaranteed)       */
            t = ((t - d) / 3) * 2;
        }

        /* Append digits in reverse order to the global array. */
        for (size_t i = 0; i < tmp_len; ++i) {
            ensure_capacity(digits_len + 1, &digits_cap, &digits);
            digits[digits_len++] = (seq_t)tmp[tmp_len - 1 - i];
        }
        /* Prepare for next integer. */
        ++next_integer;
    }

    /* Return the requested digit. */
    return digits[n];
}
