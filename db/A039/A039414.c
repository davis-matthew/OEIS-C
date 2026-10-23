/*
 * A039414  Numbers whose base-10 representation has the same nonzero number
 *          of 2's and 7's.
 * https://oeis.org/A039414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th integer whose decimal representation contains *
 *            the same non‑zero number of 2’s and 7’s. The first term is
 *            a(1)=27
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"

/* cache of already discovered terms */
static seq_t *values = NULL;      /* array of found terms   */
static size_t val_len = 0;        /* number of terms stored */
static size_t val_cap = 0;        /* current array capacity */

/* current integer being examined */
static seq_t cur = 0;             /* first value examined is 1 */

/* true if x has the same non‑zero number of digit 2 and digit 7 */
static bool has_equal_nonzero_27(seq_t x)
{
    unsigned char cnt[10] = {0};
    digit_counts(x, cnt);
    return cnt[2] > 0 && cnt[2] == cnt[7];
}

/* ensure the cache holds at least n terms, filling it by brute‑force search */
static void ensure_cache(idx_t n)
{
    while (val_len < (size_t)n) {
        /* increment cur until a number with equal 2’s and 7’s is found */
        do { ++cur; } while (!has_equal_nonzero_27(cur));
        /* grow the array if necessary */
        if (val_len == val_cap) {
            size_t new_cap = val_cap == 0 ? 16 : val_cap << 1;
            seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);  /* allocation failure */
            values = tmp;
            val_cap = new_cap;
        }
        values[val_len++] = cur;
    }
}

/* Return the n‑th term (a(1) = 27).  n must be >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined behaviour for n<1, but guard anyway */
    ensure_cache(n);
    return values[(size_t)(n - 1)];
}
