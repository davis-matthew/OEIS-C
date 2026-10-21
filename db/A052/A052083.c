/*
 * A052083  a(n)^3 is a cube whose decimal expansion's digits are grouped
 *          together.
 * https://oeis.org/A052083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   get_digits, seq_alloc_new_array, seq_free_array, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A052083 by brute‑force search.  A(n) is the
   nth non‑negative integer whose cube has all its decimal digits
   occurring in single contiguous blocks. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/get_digits.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* --------------------------------------------------------------------- */
/*  Static cache for found terms                                          */
/* --------------------------------------------------------------------- */
static seq_t *seq_cache   = NULL;   /* array of found terms          */
static size_t seq_len     = 0;      /* number of terms stored        */
static size_t seq_cap     = 0;      /* allocated capacity            */
static idx_t  next_candidate = 0;   /* next integer to test          */

/* --------------------------------------------------------------------- */
/*  Check whether all distinct decimal digits of x appear in single      */
/*  contiguous blocks.  The digit array is supplied by get_digits.       */
/* --------------------------------------------------------------------- */
static bool digits_grouped(seq_t x)
{
    uint8_t buf[256];
    size_t len;
    get_digits(x, buf, &len);          /* most significant digit first */

    bool finished[10] = { false };
    int prev = -1;                     /* sentinel for first digit */

    for (size_t i = 0; i < len; ++i) {
        int d = buf[i];
        if (d == prev) continue;       /* still inside current block */

        if (prev != -1) finished[prev] = true; /* close previous block */

        if (finished[d]) return false;  /* digit re‑appears after closing */
        prev = d;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Ensure cache has at least 'need' slots.  Grow by doubling.          */
/* --------------------------------------------------------------------- */
static void ensure_cache_capacity(size_t need)
{
    if (need <= seq_cap) return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 4;
    while (new_cap < need) new_cap *= 2;

    seq_t *tmp = seq_alloc_new_array(new_cap);
    if (!tmp) return;                   /* allocation failed – keep old state */

    if (seq_cache) {
        memcpy(tmp, seq_cache, seq_len * sizeof(seq_t));
        seq_free_array(seq_cache);
    }
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Public API:  return the nth term (n ≥ 1).  n = 0 returns 0.           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 0 */

    /* Generate terms until we have at least 'n' of them. */
    while (seq_len < (size_t)n) {
        seq_t cand   = (seq_t)next_candidate;
        seq_t cube   = seq_pow(cand, (idx_t)3);

        if (digits_grouped(cube)) {
            ensure_cache_capacity(seq_len + 1);
            seq_cache[seq_len++] = cand;
        }
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
