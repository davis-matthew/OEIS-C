/*
 * A023493  a(n) = b(n) + d(n), where b(n) = (n-th Lucas number > 1) and d(n)
 *          = (n-th non-Lucas number).
 * https://oeis.org/A023493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A023493
    a(n) = L_{n+2}  +  (n==0 ? 2 : n-th non‑Lucas number)
    where L_k is the k‑th Lucas number and the n‑th non‑Lucas number
    is the n‑th integer that is not a Lucas number (indexing from 0).
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* global cache for Lucas numbers */
static seq_t *lucas_cache = NULL;
static idx_t  lucas_cache_len = 0;
/* cache to test "is this value a Lucas number (small values)" */
static uint8_t *is_lucas_small = NULL;
static size_t   lucas_small_size = 0;
/* cache for n‑th non‑Lucas numbers (for n ≥ 1) */
static seq_t *nonlucas_arr  = NULL;  /* values ≥ 5 */
static size_t nonlucas_len = 0;   /* number of elements stored */
static size_t nonlucas_cap = 0;
static uint64_t next_nonlucas_val = 5;  /* next integer to test */

#define LIMIT 200000   /* enough for n ≤ 10000 */

/* ------------------------------------------------------------------ */
/*  Build Lucus cache up to index `idx` (inclusive).  */
static void ensure_lucas_up_to(idx_t idx)
{
    if (lucas_cache_len > idx + 1)
        return;                     /* already have it */

    /* initial allocation if necessary */
    if (lucas_cache_len == 0) {
        lucas_cache = (seq_t *)calloc(2, sizeof(seq_t));
        lucas_cache[0] = (seq_t)2;   /* L0 = 2 */
        lucas_cache[1] = (seq_t)1;   /* L1 = 1 */
        lucas_cache_len = 2;
    }

    /* extend to the requested index */
    size_t new_len = (size_t)idx + 1;
    if (lucas_cache_len < new_len) {
        lucas_cache = (seq_t *)realloc(lucas_cache, new_len * sizeof(seq_t));
        for (idx_t i = lucas_cache_len; i <= idx; ++i) {
            lucas_cache[i] = lucas_cache[i-1] + lucas_cache[i-2];
        }
        lucas_cache_len = new_len;
    }
}

static seq_t lucas_at(idx_t idx)
{
    ensure_lucas_up_to(idx);
    return lucas_cache[(size_t)idx];
}

/* ------------------------------------------------------------------ */
/*  Initialise data needed for non‑Lucas numbers. */
static void init_nonlucas_data(void)
{
    if (lucas_cache == NULL) {                 /* create Lucas cache for small values */
        lucas_cache_len = 2;
        lucas_cache    = (seq_t *)malloc(2 * sizeof(seq_t));
        lucas_cache[0] = (seq_t)2;
        lucas_cache[1] = (seq_t)1;
    }

    /* generate Lucas numbers until they exceed LIMIT, storing them in the
       small boolean table.  The total number of such values is only about
       26, so this is trivial. */
    if (is_lucas_small == NULL) {
        is_lucas_small = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
        lucas_small_size = LIMIT + 1;
        /* mark known small Lucas numbers */
        for (size_t i = 0; i < lucas_cache_len; ++i) {
            seq_t v = lucas_cache[i];
            if (v <= LIMIT)
                is_lucas_small[(size_t)v] = 1;
        }
        /* continue generating until value > LIMIT */
        idx_t i = lucas_cache_len;
        while (true) {
            seq_t next = lucas_cache[i-1] + lucas_cache[i-2];
            if (next > LIMIT)
                break;
            if (lucas_cache_len == i) {   /* expand cache if needed */
                lucas_cache = (seq_t *)realloc(lucas_cache,
                                               (i + 1) * sizeof(seq_t));
                lucas_cache_len = i + 1;
            }
            lucas_cache[i] = next;
            ++i;
            is_lucas_small[(size_t)next] = 1;
        }
    }

    /* non‑Lucas array initial capacity */
    if (nonlucas_arr == NULL) {
        nonlucas_cap = 32;
        nonlucas_arr = (seq_t *)malloc(nonlucas_cap * sizeof(seq_t));
    }
}

/* ------------------------------------------------------------------ */
/*  Grow non‑Lucas array to at least `need` elements. */
static void grow_nonlucas_arr(size_t need)
{
    if (nonlucas_cap >= need)
        return;
    size_t new_cap = nonlucas_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    nonlucas_arr = (seq_t *)realloc(nonlucas_arr, new_cap * sizeof(seq_t));
    nonlucas_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Return the n‑th non‑Lucas number (n ≥ 0). */
static seq_t nth_nonlucas(idx_t n)
{
    if (n == 0u)                 /* special value defined by OEIS */
        return (seq_t)2;

    size_t target = (size_t)n - 1;   /* because array stores [5,6,8,…] */
    if (nonlucas_len <= target) {
        grow_nonlucas_arr(target + 1);
        while (nonlucas_len <= target) {
            uint64_t val = next_nonlucas_val++;
            if (val <= LIMIT && is_lucas_small[(size_t)val])
                continue;            /* skip Lucas numbers */
            if (val <= 0)
                continue;            /* skip non‑positive (should not happen) */
            nonlucas_arr[nonlucas_len++] = (seq_t)val;
        }
    }
    return nonlucas_arr[target];
}

/* ------------------------------------------------------------------ */
/*  Main sequence function. */
seq_t a(idx_t n)
{
    init_nonlucas_data();          /* ensure helper tables are ready */

    seq_t luc = lucas_at(n + 2u);  /* L_{n+2} */
    seq_t nl  = nth_nonlucas(n);   /* n‑th non‑Lucas number */

    return luc + nl;
}
