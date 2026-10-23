/*
 * A045156  Numbers whose base-4 representation contains exactly four 2's and
 *          three 3's.
 * https://oeis.org/A045156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose base‑4 representation
 *            contains exactly four 2's and three 3's. The numbers are output
 *            in increasing order
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static state for caching computed terms */
static seq_t *sequence_cache   = NULL;   /* array of cached terms  */
static size_t sequence_cap    = 0;       /* capacity of the array   */
static size_t sequence_size   = 0;       /* number of terms cached  */
static int    next_length      = 7;      /* next base‑4 length to generate */

/* --------------------------------------------------------------------- */
/*  Grow the cache array if needed                                        */
/* --------------------------------------------------------------------- */
static void grow_cache(void)
{
    size_t new_cap = sequence_cap ? sequence_cap * 2 : 64;
    seq_t *tmp = (seq_t *)realloc(sequence_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failure */
    sequence_cache = tmp;
    sequence_cap   = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Add a value to the cache                                              */
/* --------------------------------------------------------------------- */
static void add_to_cache(seq_t val)
{
    if (sequence_size == sequence_cap) grow_cache();
    sequence_cache[sequence_size++] = val;
}

/* --------------------------------------------------------------------- */
/*  Recursive enumeration of all numbers with a given base‑4 length     */
/* --------------------------------------------------------------------- */
static void rec(int pos, int L, int cnt2, int cnt3, seq_t val)
{
    if (pos == L) {
        if (cnt2 == 4 && cnt3 == 3) add_to_cache(val);
        return;
    }

    int rem      = L - pos;
    int need2    = 4 - cnt2;
    int need3    = 3 - cnt3;
    if (need2 < 0 || need3 < 0) return;       /* too many 2 or 3 already   */

    int min_need = need2 + need3;              /* digits forced to be 2/3   */
    int d_start  = (pos == 0) ? 1 : 0;         /* no leading zero           */
    int d;

    for (d = d_start; d <= 3; ++d) {
        if (d == 2 && need2 == 0) continue;   /* no more 2's allowed      */
        if (d == 3 && need3 == 0) continue;   /* no more 3's allowed      */
        if (rem == min_need && d <= 1) continue; /* must be 2 or 3          */

        seq_t next_val = val * ((seq_t)4) + (seq_t)d;
        rec(pos + 1, L, cnt2 + (d == 2), cnt3 + (d == 3), next_val);
    }
}

/* --------------------------------------------------------------------- */
/*  Generate all numbers of a fixed base‑4 length                         */
/* --------------------------------------------------------------------- */
static void generate_length(int L)
{
    rec(0, L, 0, 0, (seq_t)0);
}

/* --------------------------------------------------------------------- */
/*  Main sequence routine                                                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* behaviour for zero index is undefined */

    /* initialise cache on first call */
    if (!sequence_cache) {
        sequence_cap = 64;
        sequence_cache = (seq_t *)malloc(sequence_cap * sizeof(seq_t));
        if (!sequence_cache) abort();
    }

    /* generate more terms until we have at least n entries */
    while (sequence_size < n) {
        generate_length(next_length);
        next_length++;          /* next base‑4 length for subsequent calls */
    }

    return sequence_cache[n - 1];
}
