/*
 * A193627  Indices n such that Perrin(n) < r^n where r is the real root of
 *          the polynomial x^3-x-1.
 * https://oeis.org/A193627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n‑th index i such that Perrin(i) < r^i,
 *  where r is the plastic constant (real root of x^3-x-1).
 *
 *  We compute Q(i)=s^i+t^i (s,t are the other two roots).
 *  Q(0)=2, Q(1)=-r, Q(2)=2-r^2.
 *  For n>=2:   Q(n) = -r*Q(n-1) - (r^2-1)*Q(n-2).
 *  a(n) is the n‑th i>=1 with Q(i)<0. */

/* required headers */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"

/* opaque types provided by the build */
extern seq_t  *seq_realloc(seq_t* old, size_t old_cap, size_t new_cap);

/* dynamic storage for the sequence */
static seq_t  *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* handle for computing the recurrence */
static bool     initialized = false;
static idx_t    last_i      = 1;           /* last computed index (i>=1) */
static long double prev_val;              /* Q(i-1) */
static long double curr_val;              /* Q(i)   */
static long double r_val;                 /* plastic constant */
static long double r_sq;                  /* r^2   */
static long double k_val;                 /* r^2-1 */

/* ------------------------------------------------------------------ */
/* compute plastic constant r (root of x^3-x-1=0) */
static void init_plastic(void)
{
    long double x = 1.32471795724474602596L;   /* good initial guess      */
    for (int i = 0; i < 30; ++i) {             /* Newton iterations      */
        long double f  = x * x * x - x - 1.0L;
        long double fp = 3.0L * x * x - 1.0L;
        x -= f / fp;
    }
    r_val = x;
    r_sq  = r_val * r_val;
    k_val = r_sq - 1.0L;

    /* initial Q values */
    prev_val = 2.0L;          /* Q(0) */
    curr_val = -r_val;        /* Q(1) */
    last_i   = 1;

    /* cache first term if it satisfies the inequality */
    if (curr_val < 0.0L) {
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)last_i;
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* grow cache to hold at least n terms */
static void ensure_cache(idx_t n)
{
    if (!initialized)              /* first call: initialise everything   */
        init_plastic();

    while (cache_len < (size_t)n) {
        /* compute next Q value using the recurrence */
        long double next_val = -r_val * curr_val - k_val * prev_val;

        /* shift the window */
        prev_val = curr_val;
        curr_val = next_val;
        ++last_i;                     /* now curr_val == Q(last_i) */

        /* record index if the condition holds */
        if (next_val < 0.0L) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_i;
        }
    }
}

/* ------------------------------------------------------------------ */
/* public function: return the n-th term of the sequence (1‑based) */
seq_t a(idx_t n)
{
    if (n == 0)                       /* undefined, but return 0 for safety */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n - 1];
}
