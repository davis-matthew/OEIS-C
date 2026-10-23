/*
 * A031018  Write 2n-1 in base 7 and juxtapose.
 * https://oeis.org/A031018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   base7_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A031018 – “Write 2n‑1 in base 7 and juxtapose”.
 *  The sequence consists of the digits of the infinite string obtained
 *  by concatenating the base‑7 representations of the odd numbers
 *  1, 3, 5, 7, 9, … .
 *
 *  a(n) is the nth digit (1‑indexed) of that string.
 *  The implementation builds the cumulative number of digits
 *  written so far for each processed odd number and uses it to
 *  locate the required digit.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/base7_len.h"

/* static storage for cumulative digit counts */
static size_t cum_cap  = 0;   /* capacity of cum array          */
static size_t cum_len  = 0;   /* number of entries currently stored */
static idx_t *cum = NULL;    /* cum[i] = total digits after odd (2*i+1) */

/* ensure that cum has at least 'need' entries */
static void
ensure_cap(size_t need)
{
    if (need <= cum_cap)
        return;
    size_t new_cap = cum_cap ? cum_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    idx_t *tmp = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
    cum = tmp;
    cum_cap = new_cap;
}

/* return the nth digit of the concatenated sequence */
seq_t
a(idx_t n)
{
    if (n == 0)           /* offset is 1, but return 0 for safety */
        return 0;

    /* extend cum until it covers position n */
    while (cum_len == 0 || cum[cum_len - 1] < n) {
        idx_t odd = 2 * (idx_t)cum_len + (idx_t)1;     /* odd number */
        idx_t len = base7_len(odd);

        ensure_cap(cum_len + 1);
        cum[cum_len] = (cum_len == 0) ? len : cum[cum_len - 1] + len;
        ++cum_len;
    }

    /* find the odd number whose representation contains position n */
    size_t i = 0;
    while (i < cum_len && cum[i] < n)
        ++i;

    idx_t prev_sum = (i == 0) ? (idx_t)0 : cum[i - 1];
    idx_t offset   = n - prev_sum - (idx_t)1;          /* 0‑based inside the number */
    idx_t odd      = 2 * (idx_t)i + (idx_t)1;
    idx_t len      = base7_len(odd);
    idx_t exp      = len - offset - (idx_t)1;

    /* compute 7^exp */
    idx_t pow = 1;
    for (idx_t j = 0; j < exp; ++j)
        pow *= (idx_t)7;

    /* extract the desired digit */
    seq_t digit = (seq_t)((odd / pow) % (idx_t)7);
    return digit;
}
