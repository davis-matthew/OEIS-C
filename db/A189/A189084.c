/*
 * A189084  Zero-one sequence based on floor(sqrt(3)): a(A022838(k))=a(k);
 *          a(A054406(k))=1-a(k); a(1)=0.
 * https://oeis.org/A189084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 125, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A189084: Zero‑one sequence based on floor(sqrt(3))
    a(1) = 0
    a(u(k)) = a(k)                      where u(k) = ⌊k·√3⌋
    a(v(k)) = 1 – a(k)                  where v(k) is the complementary Beatty
    sequence (the numbers not in u).  The function builds the Beatty tables
    up to the requested index and memoises the results. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* ----------  internal state  ---------- */
static bool      init          = false;      /* tables already initialised? */
static idx_t     cached_max    = 0;          /* largest n covered by tables */

/* mapping from integer n to the index k such that n = u(k) or n = v(k) */
static idx_t   *u_of          = NULL;         /* 0 if n ∉ u, otherwise k   */
static idx_t   *v_of          = NULL;         /* 0 if n ∉ v, otherwise k   */

/* memoised a(n) values and a flag that the value is valid */
static seq_t   *a_cache       = NULL;
static uint8_t *a_seen        = NULL;        /* 1 if a_cache[n] is computed */

/* ----------  helpers  ---------- */
static void rebuild_tables(idx_t need)
{
    idx_t new_max = need;

    if (new_max <= cached_max && init) return;      /* nothing to do */

    /* free old tables if they exist */
    free(u_of);
    free(v_of);
    free(a_cache);
    free(a_seen);

    /* allocate new tables (+1 for 1‑based indexing) */
    u_of  = (idx_t *)malloc((new_max + 1) * sizeof(idx_t));
    v_of  = (idx_t *)malloc((new_max + 1) * sizeof(idx_t));
    a_cache = (seq_t *)malloc((new_max + 1) * sizeof(seq_t));
    a_seen   = (uint8_t *)malloc((new_max + 1) * sizeof(uint8_t));

    for (idx_t i = 0; i <= new_max; ++i) {
        u_of[i] = 0;
        v_of[i] = 0;
        a_cache[i] = (seq_t)0;
        a_seen[i] = 0;
    }

    /* build u(k) = ⌊k·√3⌋ up to new_max */
    for (idx_t k = 1; ; ++k) {
        /* 3*k^2 fits comfortably in seq_t for the ranges used */
        seq_t tmp = (seq_t)3 * (seq_t)k * (seq_t)k;
        seq_t val_seq = floor_sqrt(tmp);              /* gives ⌊k·√3⌋ in seq_t   */
        idx_t val = (idx_t) val_seq;                  /* < new_max, safe cast    */
        if (val > new_max) break;
        u_of[val] = k;
    }

    /* build v(k) by scanning the numbers not in u */
    idx_t v_cnt = 0;
    for (idx_t n = 1; n <= new_max; ++n) {
        if (u_of[n] == 0) {
            ++v_cnt;
            v_of[n] = v_cnt;
        }
    }

    cached_max = new_max;
    init = true;
}

/* ----------  recursive compute with memoisation  ---------- */
static seq_t compute_a(idx_t n)
{
    if (a_seen[n]) return a_cache[n];

    seq_t res;
    if (n == 1) {
        res = (seq_t)0;
    } else if (u_of[n] != 0) {
        res = compute_a(u_of[n]);           /* n = u(k) */
    } else {
        res = (seq_t)(1 - (int)compute_a(v_of[n])); /* n = v(k) */
    }

    a_seen[n] = 1;
    a_cache[n] = res;
    return res;
}

/* ----------  public function  ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 – safe guard   */
    rebuild_tables(n);                   /* ensure tables cover n
                                            (re‑allocates if necessary) */
    return compute_a(n);
}
