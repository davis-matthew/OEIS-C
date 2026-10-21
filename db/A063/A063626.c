/*
 * A063626  Smallest k >= 0 such that 9^k has exactly n 0's in its decimal
 *          representation.
 * https://oeis.org/A063626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A063626: smallest k ≥ 0 such that 9^k has exactly n zeros in its decimal representation. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* forward declaration of the sequence function */
seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/*  helper state and tables                                           */
/* ------------------------------------------------------------------ */

#define BASE 10000U          /* decimal block base */

static uint32_t *bi = NULL;      /* big integer: least‑significant word first */
static size_t bi_len = 0;        /* number of used words              */
static size_t bi_cap = 0;        /* allocated capacity                 */

static seq_t *val = NULL;        /* minimal k for each zero count      */
static bool *seen = NULL;        /* whether zero count has been seen    */
static size_t seen_cap = 0;      /* allocated capacity for val/seen     */

static int zeros_padded[BASE];   /* zero count in 4‑digit padded block */
static int zeros_np[BASE];       /* zero count in non‑padded block      */

static bool initialized = false;

/* ------------------------------------------------------------------ */
/*  initialization                                                     */
/* ------------------------------------------------------------------ */

static void init_state(void)
{
    int i, j;

    /* precompute zero count tables */
    for (i = 0; i < (int)BASE; ++i) {
        int zpad = 0;
        int znon = 0;
        int t = i;
        /* padded representation: always 4 digits */
        for (j = 0; j < 4; ++j) {
            if ((t % 10) == 0)
                ++zpad;
            t /= 10;
        }
        zeros_padded[i] = zpad;

        /* non‑padded representation */
        if (i == 0)
            znon = 1;   /* representation "0" only occurs if the whole number is 0 */
        else {
            t = i;
            while (t > 0) {
                if ((t % 10) == 0)
                    ++znon;
                t /= 10;
            }
        }
        zeros_np[i] = znon;
    }

    /* initialise big integer 9^0 = 1 */
    bi_cap = 4;
    bi = (uint32_t *)malloc(bi_cap * sizeof(uint32_t));
    bi[0] = 1;
    bi_len = 1;

    /* initialise value/seen arrays for zero count 0 */
    seen_cap = 8;
    val = (seq_t *)malloc(seen_cap * sizeof(seq_t));
    seen = (bool *)malloc(seen_cap * sizeof(bool));
    for (i = 0; i < (int)seen_cap; ++i) {
        seen[i] = false;
        val[i]  = 0;
    }
    seen[0] = true;
    val[0]  = 0;          /* a(0) = 0 */

    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  sequence function                                                 */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (!initialized)
        init_state();

    /* if the answer is already known, return it */
    if (n < (idx_t)seen_cap && seen[(size_t)n])
        return val[(size_t)n];

    /* variables for the current state */
    static idx_t current_k = 0;     /* exponent of the current 9^k   */
    static size_t current_z = 0;    /* zero count of current 9^k     */

    /* As we may receive calls out of order, keep current_k and current_z
       up to date; they are set after each multiplication. */
    if (current_k == 0 && bi_len == 1 && bi[0] == 1)
        current_z = 0;   /* 9^0 has no zeros */

    /* iterate until we see zero count n */
    while (1) {
        /* multiply the big integer by 9 */
        uint32_t carry = 0;
        for (size_t i = 0; i < bi_len; ++i) {
            uint64_t prod = (uint64_t)bi[i] * 9U + carry;
            bi[i] = (uint32_t)(prod % BASE);
            carry  = (uint32_t)(prod / BASE);
        }
        if (carry > 0) {
            if (bi_len == bi_cap) {
                bi_cap = bi_cap * 2;
                bi = (uint32_t *)realloc(bi, bi_cap * sizeof(uint32_t));
            }
            bi[bi_len++] = carry;
        }

        ++current_k;

        /* count zeros in the new number */
        size_t z = 0;
        for (size_t i = 0; i + 1 < bi_len; ++i)
            z += zeros_padded[bi[i]];
        z += zeros_np[bi[bi_len - 1]];   /* most significant word, no leading zeros */
        current_z = z;

        /* enlarge value/seen arrays if needed */
        if (z >= seen_cap) {
            size_t new_cap = z + 8;
            val  = (seq_t *)realloc(val, new_cap * sizeof(seq_t));
            seen = (bool *)realloc(seen, new_cap * sizeof(bool));
            for (size_t i = seen_cap; i < new_cap; ++i) {
                seen[i] = false;
                val[i]  = 0;
            }
            seen_cap = new_cap;
        }

        /* record the minimal k for this zero count */
        if (!seen[z]) {
            seen[z]  = true;
            val[z] = (seq_t)current_k;
        }

        /* if we have reached the requested zero count, return */
        if (n == z)
            return val[z];
    }
}
