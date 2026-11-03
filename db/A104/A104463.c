/*
 * A104463  Complement of {A072756(n): n>=2}.
 * https://oeis.org/A104463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 62 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n‑th element of A104463 – the complement of
 * the sequence A072756 for n ≥ 2.
 *
 * Computation is done lazily.  The algorithm keeps two sequences:
 *
 *   a_vals  – the values of A072756 (called a(n) in the OEIS entry)
 *   b_vals  – the values of A104463 (the sequence to return)
 *
 * To find the next a(n) we keep a bitset `forbidden` that marks all
 * values a(k)+⌊a(k)/2⌋.  The smallest non‑negative integer not set
 * in `forbidden` is the next a(n).  When a(n) is computed, the
 * corresponding forbidden value is marked.
 *
 * For the complement sequence we keep a second bitset `used_a`
 * recording all a(n) for n ≥ 2.  The next b(n) is the smallest
 * integer not set in `used_a`.  As we enumerate b(n) we may need to
 * generate more a(n) values – the algorithm handles that automatically.
 *
 * All state is static, so repeated calls to a() work in any order.
 * The implementation only uses integer arithmetic and standard C
 * types; it works for any build defining seq_t and idx_t.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* bitset helpers ---------------------------------------------------- */
static void ensure_bitset(idx_t need, uint8_t **arr, idx_t *cap) {
    if (need < *cap) return;
    idx_t new_cap = (*cap) ? *cap << 1 : 128;
    while (new_cap <= need) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(*arr, new_cap);
    if (!tmp) abort();  /* allocation failure => crash */
    if (new_cap > *cap) memset(tmp + *cap, 0, new_cap - *cap);
    *arr  = tmp;
    *cap  = new_cap;
}

static inline void set_bit(uint8_t *arr, idx_t ix) {
    arr[ix >> 3] |= (uint8_t)(1U << (ix & 7));
}

static inline bool test_bit(const uint8_t *arr, idx_t ix) {
    return (arr[ix >> 3] >> (ix & 7)) & 1;
}

/* ------------------------------------------------------------------ */
/* --- Lazy storage of the a(n) and b(n) sequences ------------------ */
/* ------------------------------------------------------------------ */

static seq_t *a_vals   = NULL;   /* values of A072756 */
static idx_t  a_cap    = 0;
static idx_t  a_size   = 0;

static seq_t *b_vals   = NULL;   /* values of A104463 */
static idx_t  b_cap    = 0;
static idx_t  b_size   = 0;

/* ------------------------------------------------------------------ */
/* --- Bitsets used during the generation --------------------------- */
/* ------------------------------------------------------------------ */

/* forbidden sums for a(n) ----------------------------------------- */
static uint8_t *forbidden = NULL;     /* bitset for a(k)+floor(a(k)/2) */
static idx_t   forb_cap   = 0;
static idx_t   forb_next  = 0;        /* next candidate to try for a(k) */

/* used_a bitset for the complement sequence ----------------------- */
static uint8_t *used_a = NULL;        /* bitset marking a(n) for n>=2 */
static idx_t   used_a_cap = 0;

/* ------------------------------------------------------------------ */
/* --- Helper: generate a single next a(n) --------------------------- */
/* ------------------------------------------------------------------ */
static void generate_next_a(void) {
    /* find smallest non‑negative integer not forbidden */
    idx_t cand = forb_next;
    for (;;) {
        ensure_bitset(cand, &forbidden, &forb_cap);
        if (!test_bit(forbidden, cand)) break;
        cand++;
    }
    /* store the new a value */
    if (a_size >= a_cap) {
        a_cap = a_cap ? a_cap << 1 : 16;
        a_vals = mem_resize(seq_t, a_vals, a_cap);
        if (!a_vals) abort();
    }
    a_vals[a_size] = (seq_t)cand;
    /* mark a(n) for n>=2 as used for the complement */
    if (a_size >= 2) {
        ensure_bitset(cand, &used_a, &used_a_cap);
        set_bit(used_a, cand);
    }
    a_size++;

    /* mark the forbidden sum a + floor(a/2) */
    seq_t sum = a_vals[a_size-1] + (a_vals[a_size-1] / (seq_t)2);
    idx_t sum_ix = (idx_t)sum;   /* value fits into idx_t for our bounds */
    ensure_bitset(sum_ix, &forbidden, &forb_cap);
    set_bit(forbidden, sum_ix);

    forb_next = cand + 1;
}

/* ------------------------------------------------------------------ */
/* --- Helper: generate b(n) up to target index --------------------- */
/* ------------------------------------------------------------------ */
static void generate_b_up_to(idx_t target) {
    if (target < b_size) return;   /* already computed */
    /* Ensure we have the first a values to start with */
    if (a_size == 0) {
        a_cap = 16;
        a_vals = mem_new(seq_t, a_cap);
        if (!a_vals) abort();
        a_size = 0;
        /* pre‑compute a(0)=0 and a(1)=1 to initialise */
        generate_next_a();   /* a(0)=0 */
        generate_next_a();   /* a(1)=1 */
    }
    /* Prepare bitsets if not yet created */
    if (!used_a) {
        used_a_cap = 128;
        used_a = (uint8_t *)calloc(used_a_cap, 1);
        if (!used_a) abort();
    }
    if (!forbidden) {
        forb_cap = 128;
        forbidden = (uint8_t *)calloc(forb_cap, 1);
        if (!forbidden) abort();
    }
    /* mark forbidden sum for the initial a(0) and a(1) values 
     * (they were already marked inside generate_next_a) */
    /* set up scanning for b values */
    if (b_size == 0) {
        b_cap = 16;
        b_vals = mem_new(seq_t, b_cap);
        if (!b_vals) abort();
        b_size = 0;
    }
    static idx_t next_b_cand = 0;   /* smallest integer to try as b(n) */
    /* loop to produce each required b */
    for (idx_t idx = b_size; idx <= target; ++idx) {
        /* make sure we have computed all a values up to next_b_cand */
        idx_t max_a_value = (a_size>0) ? (idx_t)a_vals[a_size-1] : 0;
        while (next_b_cand > max_a_value) generate_next_a(), max_a_value = (idx_t)a_vals[a_size-1];
        /* find the next unused candidate */
        for (;;) {
            ensure_bitset(next_b_cand, &used_a, &used_a_cap);
            if (!test_bit(used_a, next_b_cand)) break;
            next_b_cand++;
            while (next_b_cand > max_a_value) generate_next_a(), max_a_value = (idx_t)a_vals[a_size-1];
        }
        /* store the b value */
        if (b_size >= b_cap) {
            b_cap = b_cap ? b_cap << 1 : 16;
            b_vals = mem_resize(seq_t, b_vals, b_cap);
            if (!b_vals) abort();
        }
        b_vals[b_size] = (seq_t)next_b_cand;
        b_size++;
        next_b_cand++;   /* start next search after this value */
    }
}

/* ------------------------------------------------------------------ */
/* --- Public API ----------------------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    generate_b_up_to(n);
    return b_vals[n];
}
