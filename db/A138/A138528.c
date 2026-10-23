/*
 * A138528  a(1) = 1. The n-th run (after the initial 1) of A138529(n) terms
 *          is (1,2,3,...,A138529(n)), where A138529(n) = Sum_{k=1..n} a(k).
 * https://oeis.org/A138528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: construct the sequence by generating successive runs.  
   Each run r has length S_r = Sum_{k=1..r} a(k).  
   The r‑th run consists of the integers 1,2,…,S_r, where the
   first element of the run is a( run_start ) = 1.  The sequence is
   built sequentially up to the requested index n.  */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state shared between calls */
    static seq_t *vals = NULL;          /* values a(i) */
    static seq_t *pref = NULL;          /* prefix sums pref[i] = Σ_{k=1..i} a(k) */
    static idx_t cap   = 0;             /* allocated size */
    static idx_t used  = 0;             /* number of elements computed so far */

    /* run management */
    static idx_t runNumber  = 0;        /* current run to be started (1‑based) */
    static idx_t runStart   = 0;        /* index where the current run begins */
    static idx_t runLen     = 0;        /* length of the current run */
    static idx_t runPos     = 0;        /* number of elements already generated in the run */

    /* initialise */
    if (used == 0) {
        cap = 64;
        vals = (seq_t*)malloc(cap * sizeof(seq_t));
        pref = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!vals || !pref) exit(EXIT_FAILURE);

        vals[1] = (seq_t)1;
        pref[1] = vals[1];
        used = 1;

        runNumber = 1;                 /* first run to generate */
        runStart  = 2;                 /* run 1 starts at index 2 */
        runLen    = (idx_t)pref[1];    /* S_1 = pref[1] = 1 */
        runPos    = 0;                 /* no element yet in the run */
    }

    /* enlarge storage if necessary */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;

        seq_t *tmp_vals = (seq_t*)realloc(vals, new_cap * sizeof(seq_t));
        seq_t *tmp_pref = (seq_t*)realloc(pref, new_cap * sizeof(seq_t));
        if (!tmp_vals || !tmp_pref) exit(EXIT_FAILURE);
        vals = tmp_vals;
        pref = tmp_pref;
        cap = new_cap;
    }

    /* generate terms up to n */
    for (idx_t i = used + 1; i <= n; ++i) {
        if (i == runStart) {           /* start of a new run */
            vals[i] = (seq_t)1;
            pref[i] = pref[i - 1] + vals[i];

            runPos = 1;                /* first element has just been written */
            if (runPos == runLen) {    /* run of length 1 finished immediately */
                /* prepare next run */
                runNumber++;
                runLen = (idx_t)pref[runNumber];
                runStart = i + 1;
                runPos = 0;
            }
        } else {                       /* continuation inside the current run */
            vals[i] = (seq_t)(runPos + 1);
            pref[i] = pref[i - 1] + vals[i];
            runPos++;

            if (runPos == runLen) {    /* run completed */
                runNumber++;
                runLen = (idx_t)pref[runNumber];
                runStart = i + 1;
                runPos = 0;
            }
        }
    }

    used = n;
    return vals[n];
}
