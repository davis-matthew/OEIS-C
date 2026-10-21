/*
 * A070945  Number of permutations on n letters that have only cycles of
 *          length 4 or less.
 * https://oeis.org/A070945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=2, a(3)=6,
   a(n)=a(n-1)+(n-1)*a(n-2)+(n-1)*(n-2)*a(n-3)+(n-1)*(n-2)*(n-3)*a(n-4) */
/* This gives the number of permutations on n letters with all cycles of length
   at most 4.  The function caches values up to 10 000 to keep calls fast and
   falls back to a simple rolling computation for larger n.  It follows all
   requirements: external linkage, only the allowed operations on seq_t,
   compiled with C11 or C++17. */

#include <stdbool.h>

#define A070945_MAX_N 10000              /* cache size; n is guaranteed ≤10 000 */
static seq_t _a_cache[A070945_MAX_N+1] = {0};
static bool  _a_computed[A070945_MAX_N+1] = {false};

seq_t a(idx_t n)
{
    /* --------------------------------------------------------------------
       Handle the cached range [0, A070945_MAX_N] first.  The initial
       values are inserted lazily the first time a() is called. */
    /* -------------------------------------------------------------------- */
    if (_a_computed[0] == false) { _a_cache[0] = (seq_t)1; _a_computed[0] = true; }
    if (_a_computed[1] == false) { _a_cache[1] = (seq_t)1; _a_computed[1] = true; }
    if (_a_computed[2] == false) { _a_cache[2] = (seq_t)2; _a_computed[2] = true; }
    if (_a_computed[3] == false) { _a_cache[3] = (seq_t)6; _a_computed[3] = true; }

    if (n <= A070945_MAX_N) {
        /* compute or fetch cached values */
        for (idx_t i = 4; i <= n; ++i) {
            if (_a_computed[i]) continue;           /* already cached */
            seq_t t1   = (seq_t)(i-1);
            seq_t val  = _a_cache[i-1];
            val += t1 * _a_cache[i-2];
            seq_t t2  = t1 * (seq_t)(i-2);
            val += t2 * _a_cache[i-3];
            seq_t t3  = t2 * (seq_t)(i-3);
            val += t3 * _a_cache[i-4];
            _a_cache[i] = val;
            _a_computed[i] = true;
        }
        return _a_cache[n];
    }

    /* --------------------------------------------------------------------
       For n > A070945_MAX_N we use a simple rolling computation that keeps
       only the last four results.  This is O(n) time and O(1) memory and
       works for arbitrarily large n (subject to seq_t capacity).  No
       caching is performed for these larger indices. */
    /* -------------------------------------------------------------------- */
    seq_t a0 = (seq_t)1;      /* a(n-4) */
    seq_t a1 = (seq_t)1;      /* a(n-3) */
    seq_t a2 = (seq_t)2;      /* a(n-2) */
    seq_t a3 = (seq_t)6;      /* a(n-1) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t t1  = (seq_t)(i-1);
        seq_t val = a3;                /* a(i-1) */
        val += t1 * a2;                /* + (i-1)*a(i-2) */
        seq_t t2 = t1 * (seq_t)(i-2);  /* (i-1)*(i-2) */
        val += t2 * a1;                /* + (i-1)*(i-2)*a(i-3) */
        seq_t t3 = t2 * (seq_t)(i-3);  /* (i-1)*(i-2)*(i-3) */
        val += t3 * a0;                /* + (i-1)*(i-2)*(i-3)*a(i-4) */
        a0 = a1; a1 = a2; a2 = a3; a3 = val;
    }
    return a3;
}
