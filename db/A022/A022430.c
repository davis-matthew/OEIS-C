/*
 * A022430  a(n) = c(n-1) + c(n-3) where c is the sequence of numbers not in
 *          a.
 * https://oeis.org/A022430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A022430
 *
 * a(n) = c(n-1) + c(n-3),  where c is the sequence of
 * natural numbers not belonging to a.
 * Offset 0 (a(0)=1).
 * The function is memoised in static tables and works
 * for any order of queries.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Static tables for the sequence and its complement */
static seq_t *a_seq      = NULL;   /* a(n) */
static size_t a_len      = 0;
static size_t a_cap      = 0;

static seq_t *c_seq      = NULL;   /* c(n) */
static size_t c_len      = 0;
static size_t c_cap      = 0;

/* Bitmap of numbers that are in a */
static uint8_t *present  = NULL;
static size_t present_cap = 0;

/* Next integer to test for being missing from a */
static idx_t next_missing_candidate = 2; /* will become 3 on first use */

/* Macro to grow an array to at least 'need' elements */
#define ENSURE_CAP(arr,cap,need)                              \
    do {                                                      \
        if ((cap) < (size_t)(need)) {                        \
            size_t new_cap = (cap) ? (cap) : 4;               \
            while (new_cap < (size_t)(need)) new_cap <<= 1;  \
            seq_t *tmp = (seq_t*)realloc((arr), new_cap * sizeof(seq_t)); \
            if (!tmp) abort();                               \
            (arr) = tmp;                                     \
            (cap) = new_cap;                                 \
        }                                                     \
    } while (0)

/* Ensure present bitmap is large enough for index n */
static void ensure_present_cap(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need > present_cap) {
        size_t old_cap = present_cap;
        size_t new_cap = present_cap ? present_cap : 16;
        while (new_cap < need) new_cap <<= 1;
        uint8_t *tmp = (uint8_t*)realloc(present, new_cap * sizeof(uint8_t));
        if (!tmp) abort();
        /* zero newly allocated area */
        memset(tmp + old_cap, 0, new_cap - old_cap);
        present = tmp;
        present_cap = new_cap;
    }
}

/* Initialise static storage with the first few terms */
static void init_static(void)
{
    /* a initial values */
    a_cap = 4;
    a_seq = (seq_t*)malloc(a_cap * sizeof(seq_t));
    if (!a_seq) abort();
    a_seq[0] = (seq_t)1;
    a_seq[1] = (seq_t)2;
    a_seq[2] = (seq_t)6;
    a_len = 3;

    /* empty c array */
    c_cap = 4;
    c_seq = (seq_t*)malloc(c_cap * sizeof(seq_t));
    if (!c_seq) abort();

    /* present bitmap */
    present_cap = 16;
    present = (uint8_t*)calloc(present_cap, sizeof(uint8_t));
    if (!present) abort();
    /* mark the initial a values as present */
    present[(size_t)1] = 1;
    present[(size_t)2] = 1;
    present[(size_t)6] = 1;  /* a(2) */

    /* next missing candidate will start at 3 (incremented on first use) */
    next_missing_candidate = 2;

    /* No further initialisation needed */
}

/* Function required by the problem statement */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_static();
        initialized = true;
    }

    if ((idx_t)a_len > n) return a_seq[(size_t)n];

    for (size_t cur = a_len; cur <= (size_t)n; ++cur) {
        /* Ensure c(cur‑1) and c(cur‑3) are available */
        while (c_len <= cur - 1) {
            ++next_missing_candidate;
            ensure_present_cap(next_missing_candidate);
            if (!present[(size_t)next_missing_candidate]) {
                ENSURE_CAP(c_seq, c_cap, c_len + 1);
                c_seq[c_len++] = (seq_t)next_missing_candidate;
            }
        }

        /* Compute a[cur] = c[cur‑1] + c[cur‑3] */
        seq_t val = c_seq[cur - 1] + c_seq[cur - 3];

        ENSURE_CAP(a_seq, a_cap, cur + 1);
        a_seq[cur] = val;

        /* Mark this new value as present */
        idx_t val_idx = (idx_t)val;          /* safe for n ≤ 10000 */
        ensure_present_cap(val_idx);
        present[(size_t)val_idx] = 1;
    }

    return a_seq[(size_t)n];
}
