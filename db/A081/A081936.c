/*
 * A081936  Odd short legs of primitive Pythagorean triangles sorted on
 *          semiperimeter.
 * https://oeis.org/A081936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd short leg of the nth primitive Pythagorean triangle,
 *            sorted by ascending semiperimeter. The sequence is generated
 *            lazily the first time it is needed
 * Range:     u8 n <= 36, u16 n <= 7971, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd_idx.h"

/* ------------------------------------------------------------- */
/* Helper types and functions                                   */
/* ------------------------------------------------------------- */

typedef struct {
    uint64_t s;   /* semiperimeter = m*(m+n)              */
    seq_t   leg;  /* odd short leg (m^2 - n^2)            */
} Entry;

/* gcd for idx_t (unsigned 64‑bit) */

/* ------------------------------------------------------------- */
/* Dynamic array for Entry objects                              */
/* ------------------------------------------------------------- */

static Entry *entries = NULL;
static size_t entries_len = 0, entries_cap = 0;

/* enlarge the entries array if needed            */
static void ensure_entries_cap(size_t need)
{
    if (entries_cap >= need) return;
    size_t new_cap = entries_cap ? entries_cap * 2 : 1024;
    while (new_cap < need) new_cap <<= 1;
    Entry *tmp = (Entry *)realloc(entries, new_cap * sizeof(Entry));
    if (!tmp) abort();
    entries = tmp;
    entries_cap = new_cap;
}

/* append a new entry to the array                 */
static void append_entry(uint64_t s, seq_t leg)
{
    ensure_entries_cap(entries_len + 1);
    entries[entries_len].s   = s;
    entries[entries_len].leg = leg;
    entries_len++;
}

/* comparison for qsort (by semiperimeter)        */
static int cmp_entry(const void *a, const void *b)
{
    const Entry *ea = (const Entry *)a;
    const Entry *eb = (const Entry *)b;
    if (ea->s < eb->s) return -1;
    if (ea->s > eb->s) return 1;
    return 0;
}

/* ------------------------------------------------------------- */
/* Cache for the final sequence                                */
/* ------------------------------------------------------------- */

static seq_t *seq = NULL;
static size_t seq_len = 0;
static int ready = 0;

/* ------------------------------------------------------------- */
/* Build the entire sequence once                            */
/* ------------------------------------------------------------- */

static void build_sequence(void)
{
    /* Choose an upper bound for the parameter m that delivers
       more than enough entries for n up to at least 10 000.   */
    const idx_t MAX_M = 2000;

    for (idx_t m = 2; m <= MAX_M; ++m) {
        for (idx_t n = 1; n < m; ++n) {
            if (((m - n) & 1) && gcd_idx(m, n) == 1) {
                /* Primitive triple: a = m^2 – n^2, b = 2mn, c = m^2 + n^2   */
                uint64_t a = (uint64_t)m * (uint64_t)m
                           - (uint64_t)n * (uint64_t)n;
                uint64_t b = 2ULL * (uint64_t)m * (uint64_t)n;
                /* short leg a, odd short leg             */
                if (a < b && (a & 1)) {
                    uint64_t s = (uint64_t)m * (uint64_t)(m + n); /* semiperimeter */
                    append_entry(s, (seq_t)a);
                }
            }
        }
    }

    /* Sort all entries by semiperimeter */
    qsort(entries, entries_len, sizeof(Entry), cmp_entry);

    /* Transfer the legs into the final sequence array */
    seq = (seq_t *)malloc(entries_len * sizeof(seq_t));
    if (!seq) abort();
    for (size_t i = 0; i < entries_len; ++i) {
        seq[i] = entries[i].leg;
    }
    seq_len = entries_len;
    ready = 1;
}

/* ------------------------------------------------------------- */
/* Public interface                                            */
/* ------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (!ready) build_sequence();
    if (n == 0 || n > seq_len) abort();   /* index out of range */
    return seq[n - 1];
}
