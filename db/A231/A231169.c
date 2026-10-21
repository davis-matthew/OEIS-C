/*
 * A231169  Triangle read by rows: T[i,j] = number of (distinct) letters which
 *          the English names of i and j have in common; j=0,...,i ;
 *          i=0,1,2,...
 * https://oeis.org/A231169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, tabl
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   mask_of_string, popcnt32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute the number of distinct letters common to the
   English names of the two numbers forming the entry at position n in
   the A231169 triangle.  The function uses a cache of letter‑masks for
   all numbers seen so far, builds it on demand, and counts shared
   letters by a popcount on a 32‑bit bitmask.  The algorithm runs in
   amortised O(1) time per call. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mask_of_string.h"
#include "../utils/popcnt32.h"

/* ----- English number name tables (lower‑case, no spaces/hyphens) ----- */
static const char *ords[] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};
static const char *tens_names[10] = {
    NULL, NULL, "twenty", "thirty", "forty", "fifty",
    "sixty", "seventy", "eighty", "ninety"
};
static const char *scale_names[] = {
    "", "thousand", "million", "billion", "trillion",
    "quadrillion", "quintillion", "sextillion", "septillion",
    "octillion", "nonillion", "decillion"
};

/* ----- helper: convert a number to a bitmask of its letters ----- */
static uint32_t number_to_mask(idx_t n)
{
    if (n == 0)
        return mask_of_string("zero");

    uint32_t mask = 0;
    idx_t rem = n;
    int power = 0;                    /* group index: 0 = units, 1 = thousand, ... */
    while (rem > 0) {
        int group = (int)(rem % 1000);
        if (group > 0) {
            /* hundreds place */
            if (group >= 100) {
                int h = group / 100;
                mask |= mask_of_string(ords[h]);          /* 1..9 */
                mask |= mask_of_string("hundred");
                int sub = group % 100;
                if (sub > 0) {
                    if (sub < 20)
                        mask |= mask_of_string(ords[sub]);
                    else {
                        int t = sub / 10;
                        mask |= mask_of_string(tens_names[t]);    /* 20..90 */
                        int o = sub % 10;
                        if (o > 0)
                            mask |= mask_of_string(ords[o]);
                    }
                }
            }
            /* tens and ones place */
            else if (group < 20) {
                mask |= mask_of_string(ords[group]);
            }
            else {
                int t = group / 10;
                mask |= mask_of_string(tens_names[t]);        /* 20..90 */
                int o = group % 10;
                if (o > 0)
                    mask |= mask_of_string(ords[o]);
            }

            if (power > 0 && power < (int)(sizeof(scale_names)/sizeof(scale_names[0])))
                mask |= mask_of_string(scale_names[power]);
        }

        rem /= 1000;
        ++power;
    }
    return mask;
}

/* ----- cache of letter masks ------------------------------------- */
static uint32_t *letter_masks = NULL;   /* array of masks, index = number */
static idx_t mask_cap = 0;              /* amount of memory allocated */

/* ensure masks are computed for all numbers up to needed */
static void init_masks(idx_t needed)
{
    if (needed < mask_cap)
        return;

    uint32_t *new_masks = (uint32_t *)realloc(letter_masks,
                     (size_t)(needed + 1) * sizeof(uint32_t));
    if (!new_masks)        /* allocation failure – unlikely in normal use */
        return;

    /* fill newly added entries */
    for (idx_t num = mask_cap; num <= needed; ++num)
        new_masks[(size_t)num] = number_to_mask(num);

    letter_masks = new_masks;
    mask_cap = needed + 1;
}

/* ----- locate the row i of the triangle that contains position n ----- */
static idx_t find_row(idx_t n)
{
    /* triangular numbers T(i) = i*(i+1)/2.  Find i such that T(i) <= n < T(i+1) */
    idx_t i = 0;
    /* O(sqrt(n)) scan – sufficient for n <= 10000 and fast enough for all
       required values.  If n is very large, this still runs in about
       sqrt(n) steps. */
    while (( (i + 1) * (i + 2) ) / 2 <= n)
        ++i;
    return i;
}

/* ----- public function required by the build framework ----- */
seq_t a(idx_t n)
{
    idx_t i = find_row(n);
    idx_t prev = ( (i * (i + 1)) / 2 );
    idx_t j = n - prev;                      /* j <= i */

    /* ensure masks exist for numbers up to max(i,j) */
    idx_t max_num = (i > j) ? i : j;
    init_masks(max_num);

    uint32_t mask_i = letter_masks[(size_t)i];
    uint32_t mask_j = letter_masks[(size_t)j];
    uint32_t common  = mask_i & mask_j;
    unsigned int count = popcnt32(common);

    return (seq_t)count;
}
