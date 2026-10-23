/*
 * A037324  Numbers whose base-5 and base-8 expansions have the same digit
 *          sum.
 * https://oeis.org/A037324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A037324: Numbers whose base-5 and base-8 expansions have the same digit sum.
 *
 * Computation: a value x is accepted if digit_sum_base5(x) == digit_sum_base8(x).
 * The sequence is generated in ascending order by iterating over consecutive integers,
 * updating the base-5 and base-8 digit representations incrementally and recomputing
 * the digit sums. Results are memoised so that any call to a(n) is answered in O(1)
 * after the required terms have been generated.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Grow a dynamic array of seq_t elements.  The pointer should be a pointer
   to a seq_t*.  New capacity is at least 'need'.  The allocation size must
   be multiples of sizeof(seq_t), not sizeof(void*). */
static void grow_seq(void **arr, size_t *cap, size_t need)
{
    if (need > *cap) {
        size_t newcap = *cap ? (*cap << 1) : 16;
        if (newcap < need) newcap = need;
        *arr = realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/*-----------------------------------------------------------------------------*/
/* Global state for the memoised sequence generator */
static seq_t *memo = NULL;          /* array holding the found sequence values   */
static size_t  memo_cap = 0;        /* allocated capacity of 'memo'             */
static size_t  memo_len = 0;        /* number of values stored so far           */
static idx_t   cur_val   = 1;       /* current integer being examined          */

/* Base‑5 and base‑8 digit arrays (least‑significant digit first) */
static uint8_t d5[64], d8[64];
static size_t  len5 = 1, len8 = 1;

/* Current digit sums in base 5 and base 8 */
static uint32_t sum5 = 1, sum8 = 1;

/* Increment the current integer in base‑5 representation. */
static void incr_base5(void)
{
    size_t i = 0;
    while (1) {
        d5[i]++;
        if (d5[i] == 5) {
            d5[i] = 0;
            i++;
            if (i == len5) {
                d5[i] = 1;
                len5++;
                break;
            }
        } else
            break;
    }
}

/* Increment the current integer in base‑8 representation. */
static void incr_base8(void)
{
    size_t i = 0;
    while (1) {
        d8[i]++;
        if (d8[i] == 8) {
            d8[i] = 0;
            i++;
            if (i == len8) {
                d8[i] = 1;
                len8++;
                break;
            }
        } else
            break;
    }
}

/* Recompute sum5 and sum8 from the current digit arrays. */
static void recompute_sums(void)
{
    uint32_t s = 0;
    for (size_t i = 0; i < len5; i++) s += d5[i];
    sum5 = s;
    s = 0;
    for (size_t i = 0; i < len8; i++) s += d8[i];
    sum8 = s;
}

/* Initialise global state on the first call. */
static void init_state(void)
{
    if (memo != NULL) return; /* already initialised */

    memo = (seq_t*)malloc(128 * sizeof(seq_t));
    memo_cap = 128;
    memo_len = 0;

    /* Start with value 1 */
    cur_val = 1;
    d5[0] = 1;
    d8[0] = 1;
    len5 = len8 = 1;
    sum5 = sum8 = 1;

    if (sum5 == sum8) {
        memo[memo_len++] = (seq_t)cur_val;
    }
}

/*-----------------------------------------------------------------------------*/
/* Public function required by the framework */
seq_t a(idx_t n)
{
    init_state();

    /* Generate terms until we have at least n of them */
    while (memo_len < (size_t)n) {
        /* Advance to the next integer */
        cur_val++;
        incr_base5();
        incr_base8();
        recompute_sums();

        if (sum5 == sum8) {
            /* Store the found value, growing the array if necessary */
            grow_seq((void**)&memo, &memo_cap, memo_len + 1);
            memo[memo_len++] = (seq_t)cur_val;
        }
    }

    return memo[(size_t)n - 1];
}
