/*
 * A103953  Smallest perfect power b^e such that b^e+n is also a perfect
 *          power, or 0 if no such perfect power exists.
 * https://oeis.org/A103953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest perfect power P such that P+n is also a perfect
 *            power; if no such P exists, a(n)=0
 * Range:     u8 none, u16 n <= 29, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   comp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/comp_seq.h"

#define MAX_POWER_LIMIT 1000000000000ULL   /* limit for pre‑computed perfect powers */

/* ------------------------------------------------------------------ */
/*  Storage for all perfect powers up to MAX_POWER_LIMIT.  */
/* ------------------------------------------------------------------ */
static seq_t *pp = NULL;   /* array of distinct perfect powers (ascending) */
static idx_t pp_cnt = 0;   /* number of stored perfect powers */

/* ------------------------------------------------------------------ */
/*  Comparator for qsort used on pp.  Compatible both in C and C++.  */

/* ------------------------------------------------------------------ */
/*  Binary search in pp; true if the value is a perfect power.  */
/*  1 is treated as a perfect power although it is not in the array. */
/* ------------------------------------------------------------------ */
static bool pp_contains(seq_t v)
{
    if (v == (seq_t)1) return true;
    idx_t lo = 0, hi = pp_cnt;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (pp[mid] < v) lo = mid + 1;
        else hi = mid;
    }
    return (lo < pp_cnt && pp[lo] == v);
}

/* ------------------------------------------------------------------ */
/*  Initialise the pretty‑small perfect‑power table.  */
/* ------------------------------------------------------------------ */
static void init_pp(void)
{
    if (pp_cnt != 0) return;          /* already initialised */

    /* initial capacity – will grow if needed */
    idx_t cap = 2000000;
    pp = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!pp) return;                  /* out of memory – unlikely in tests */

    seq_t limit = (seq_t)MAX_POWER_LIMIT;

    for (seq_t base = 2; ; ++base) {
        /* stop if the smallest exponent (2) already exceeds the limit */
        if (base > limit / base) break;
        seq_t val = base * base;     /* exponent = 2 */
        while (1) {
            if (pp_cnt >= cap) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(pp, cap * sizeof(seq_t));
                if (!tmp) return;
                pp = tmp;
            }
            pp[pp_cnt++] = val;
            if (val > limit / base) break;
            val *= base;
        }
    }

    /* sort and keep only distinct values */
    qsort(pp, pp_cnt, sizeof(seq_t), comp_seq);
    idx_t uniq = 0;
    for (idx_t i = 0; i < pp_cnt; ++i) {
        if (uniq == 0 || pp[i] != pp[uniq - 1]) {
            pp[uniq++] = pp[i];
        }
    }
    pp_cnt = uniq;
}

/* ------------------------------------------------------------------ */
/*  Find the first index in pp with value >= target.               */
/* ------------------------------------------------------------------ */
static idx_t first_ge(seq_t target)
{
    idx_t lo = 0, hi = pp_cnt;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (pp[mid] < target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/*  Required sequence function.   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_pp();                                     /* ensure table exists */
    seq_t target = (seq_t)n + 1;                   /* Q must be > n */
    idx_t i = first_ge(target);

    for (; i < pp_cnt; ++i) {
        seq_t q = pp[i];
        seq_t p = q - (seq_t)n;
        if (p == (seq_t)0) continue;                /* just in case */
        if (pp_contains(p)) return p;
    }
    return 0;                                      /* no solution found */
}
