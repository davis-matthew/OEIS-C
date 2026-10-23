/*
 * A115949  a(1)=1 and a(n)=least positive integer not yet in sequence, if n
 *          is even and a(n) = sum of least two positive integers not yet in
 *          sequence, if n is odd.
 * https://oeis.org/A115949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least missing if n even; sum of two least missing if n
 *            odd, a(1) = 1. Constructed by explicit simulation with a
 *            used‑bit array and memoised result vector. Implements O(n) time
 *            and memory for any n, with out‑of‑order queries supported
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *seq      = NULL;   /* memoised values                         */
static size_t seq_len  = 0;       /* number of computed terms (a(1) .. a(n)) */
static size_t seq_cap  = 0;       /* capacity of seq                         */

static uint8_t *used   = NULL;   /* used[i] == 1 iff i already in seq      */
static size_t used_cap = 0;       /* capacity of used array                 */
static seq_t next_missing = 1;    /* smallest unused integer                */

/* enlarge used array so that index v is addressable */
static void ensure_used_capacity(seq_t v)
{
    uint64_t val = (uint64_t)v;
    if (val < used_cap) return;

    size_t new_cap = used_cap ? used_cap << 1 : 128;
    while (new_cap <= val) new_cap <<= 1;

    uint8_t *tmp = (uint8_t*)realloc(used, new_cap * sizeof *used);
    if (!tmp) abort();
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

/* mark integer v as used and advance next_missing while needed */
static void mark_used(seq_t v)
{
    ensure_used_capacity(v);
    used[(uint64_t)v] = 1;

    if ((uint64_t)v == (uint64_t)next_missing) {
        do {
            ++next_missing;
            if ((uint64_t)next_missing > (uint64_t)(used_cap - 1))
                ensure_used_capacity(next_missing);
        } while (used[(uint64_t)next_missing]);
    }
}

/* enlarge seq vector to hold at least n entries */
static void ensure_seq_capacity(size_t n)
{
    if (n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 128;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof *seq);
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* compute a(n) for any n >= 1, supporting arbitrary call order */
seq_t a(idx_t n)
{
    if (n <= 0) return 0;

    /* initialise on first call */
    if (seq == NULL) {
        seq_cap = 128;
        seq = (seq_t*)malloc(seq_cap * sizeof *seq);
        if (!seq) abort();

        used_cap = 128;
        used = (uint8_t*)calloc(used_cap, sizeof *used);
        if (!used) abort();

        next_missing = 1;
        seq_len = 0;
    }

    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 1;                      /* seed a(1) = 1 */
        } else if ((i & 1) == 0) {        /* even index  -> single missing  */
            val = next_missing;
        } else {                          /* odd  index  -> sum of two missing */
            seq_t m1 = next_missing;
            seq_t m2 = m1 + 1;
            while (1) {
                ensure_used_capacity(m2);
                if (!used[(uint64_t)m2]) break;
                ++m2;
            }
            val = m1 + m2;
        }

        ensure_seq_capacity(seq_len + 1);
        seq[seq_len++] = val;
        mark_used(val);
    }

    return seq[n - 1];
}
