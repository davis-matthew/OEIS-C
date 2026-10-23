/*
 * A198375  Smallest n-digit number whose product of digits is n or 0 if no
 *          number exists.
 * https://oeis.org/A198375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest n‑digit integer whose product of digits equals n
 *            (0 if no such number exists)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Representation of a product in terms of digits > 1.  The digits are
   kept sorted so that lexicographic comparison is trivial. */
struct best {
    int  len;                 /* number of digits > 1 that achieve the product */
    unsigned char digits[16]; /* sorted digits in ascending order          */
};

static struct best *best_arr = NULL; /* array indexed by product value      */
static idx_t  comp_max = 0;          /* largest product precomputed so far  */

/* --- Helper: make the DP table large enough and fill new entries --- */
static void ensure(idx_t needed)
{
    if (needed <= comp_max)
        return;

    size_t new_cap = (size_t)(needed + 1); /* indices 0 … needed */
    struct best *tmp = (struct best *)realloc(best_arr, new_cap * sizeof(struct best));
    if (!tmp)
        exit(1);                          /* out of memory */
    best_arr = tmp;

    /* Initialise newly allocated slots to "impossible" */
    for (idx_t i = comp_max + 1; i <= needed; ++i)
        best_arr[i].len = -1;
    /* Base case: product 1 requires no digits > 1 */
    if (needed >= 1)
        best_arr[1].len = 0;

    /* Dynamic‑programming: build solutions for products comp_max+1 … needed */
    idx_t start = comp_max + 1;
    if (start < 2)
        start = 2;                         /* product 1 is already handled */

    for (idx_t p = start; p <= needed; ++p) {
        best_arr[p].len = -1;              /* mark impossible first */

        for (int d = 2; d <= 9; ++d) {
            if (p % d != 0)              /* d not a divisor of p */
                continue;
            idx_t prev = p / (idx_t)d;
            if (best_arr[prev].len < 0)   /* no representation for prev */
                continue;

            int newlen = best_arr[prev].len + 1;
            unsigned char temp[16];
            int i = 0, j = 0;
            bool inserted = false;

            /* Merge digit d into the sorted previous digits */
            while (i < best_arr[prev].len) {
                if (!inserted && d <= best_arr[prev].digits[i]) {
                    temp[j++] = (unsigned char)d;
                    inserted = true;
                }
                temp[j++] = best_arr[prev].digits[i++];
            }
            if (!inserted)                 /* d is larger than all previous */
                temp[j++] = (unsigned char)d;

            /* Decide whether this representation is better */
            bool better = false;
            int oldlen = best_arr[p].len;
            if (oldlen == -1) {
                better = true;
            } else if (newlen < oldlen) {
                better = true;
            } else if (newlen == oldlen) {
                for (int k = 0; k < newlen; ++k) {
                    if (temp[k] < best_arr[p].digits[k]) {
                        better = true;
                        break;
                    }
                    if (temp[k] > best_arr[p].digits[k])
                        break;
                }
            }

            if (better) {
                best_arr[p].len = newlen;
                memcpy(best_arr[p].digits, temp, (size_t)newlen);
            }
        }
    }

    comp_max = needed;
}

/* --- The required OEIS sequence function --- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                          /* out of sequence bounds */

    ensure(n);

    if (best_arr[n].len < 0)
        return 0;                          /* no representation → answer 0 */

    /* Build the number: leading 1's followed by the stored digits */
    seq_t  res = 0;
    idx_t  leading = n - (idx_t)best_arr[n].len;
    for (idx_t i = 0; i < leading; ++i)
        res = res * 10 + 1;
    for (int j = 0; j < best_arr[n].len; ++j)
        res = res * 10 + (seq_t)best_arr[n].digits[j];

    return res;
}
