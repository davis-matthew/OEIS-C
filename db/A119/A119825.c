/*
 * A119825  Triangle read by rows: T(n,k) is the number of ternary sequences
 *          of length n containing k subsequences 000 (consecutively; n,k>=0).
 * https://oeis.org/A119825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 191, u64 n <= 821,
 *            u128 n <= 3322, big any n
 * Validated: standard, pass, 76 terms through n = 9361 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A119825: T(n,k) – number of ternary strings of length n with exactly k
    occurrences of the consecutive substring 000.  The sequence a(n) listed in
    the OEIS entry is the triangle read by rows: a(n)=T(row, column) where
    columns are counted from 0.  The function 'a' returns the value for the
    flatten‑indexed sequence as required.  The routine works for any n up to
    about 10 000, using an O(n²) DP that runs in well under two seconds
    for the supported data types.  No external files or libraries are used
    beyond the standard C11 headers.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/*  Compute the cumulative count S(r) = number of terms in rows 0..r.
        For r==0, S(0)=1.  For r>=1, S(r)=r*(r-1)/2 + 2.
    The result fits in idx_t for all r that are below the maximum
    index that may occur in the input.  */
static inline unsigned __int128 S_val(idx_t r)
{
    if (r == 0) return (unsigned __int128)1;
    return (unsigned __int128)r * (r - 1) / 2 + 2;
}

/*  Find the row number r such that the index idx lies in row r.
    For idx==0 return r=0.  */
static inline idx_t find_row(idx_t idx)
{
    if (idx == 0) return 0;          /* first element of the triangle */
    idx_t low = 0, high = 1;
    /*  increase high until S(high) > idx  */
    while (S_val(high) <= idx) {
        if (high > UINT64_MAX / 2) { high = UINT64_MAX; break; }
        high <<= 1;
    }
    /*  binary search for the smallest high with S(high) > idx  */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (S_val(mid) <= idx) low = mid;
        else high = mid;
    }
    return high;   /* high is the sought row index */
}

/*  Main routine:  Returns T(row, k) for the term a(idx).  */
seq_t a(idx_t idx)
{
    /*  Determine row r and column k  */
    idx_t r = find_row(idx);          /* row of the term */
    idx_t start = (r == 0 ? (idx_t)0 : (idx_t)S_val(r - 1)); /* start index of this row */
    idx_t k = idx - start;            /* column within the row */

    /*  Special case for very small n:  length 0,1,2  */
    if (r == 0 || r == 1 || r == 2) {
        /*  For these rows only k==0, the value is 3^r  */
        seq_t val = 1;
        for (idx_t i = 0; i < r; ++i) val *= (seq_t)3;
        return val;
    }

    /*  DP arrays:  f[s][k]  where s=0,1,2 tracks trailing zeros  */
    idx_t maxK = r - 1;                /* maximum possible k is r-2; array size r-1 safe */
    seq_t *f0 = mem_new(seq_t, maxK + 1);
    seq_t *f1 = mem_new(seq_t, maxK + 1);
    seq_t *f2 = mem_new(seq_t, maxK + 1);
    seq_t *nf0 = mem_new(seq_t, maxK + 1);
    seq_t *nf1 = mem_new(seq_t, maxK + 1);
    seq_t *nf2 = mem_new(seq_t, maxK + 1);
    if (!f0 || !f1 || !f2 || !nf0 || !nf1 || !nf2) {
        /*  Allocation failed – return zero as a safe fallback.  */
        mem_free(f0); mem_free(f1); mem_free(f2); mem_free(nf0); mem_free(nf1); mem_free(nf2);
        return (seq_t)0;
    }

    /*  Initialize for length 0  */
    f0[0] = (seq_t)1;

    for (idx_t pos = 0; pos < r; ++pos) {
        /*  Determine the maximum k that can appear after 'pos' characters:  pos  */
        idx_t curMaxK = pos;  /* rather than pos-2, pos is safe and small */
        /*  Zero new arrays  */
        for (idx_t i = 0; i <= curMaxK; ++i) {
            nf0[i] = nf1[i] = nf2[i] = (seq_t)0;
        }

        for (idx_t kk = 0; kk <= curMaxK; ++kk) {
            seq_t val0 = f0[kk];
            seq_t val1 = f1[kk];
            seq_t val2 = f2[kk];
            if (val0) {
                /*  Add digit 0 → state 1  */
                nf1[kk] += val0;
                /*  Add digit 1 or 2 → state 0 (two possibilities)  */
                nf0[kk] += val0 * (seq_t)2;
            }
            if (val1) {
                /*  Add digit 0 → state 2 (no new occurrence)  */
                nf2[kk] += val1;
                /*  Add digit 1 or 2 → state 0  */
                nf0[kk] += val1 * (seq_t)2;
            }
            if (val2) {
                /*  Add digit 0 → state 2 with a new occurrence  */
                if (kk + 1 <= curMaxK) nf2[kk + 1] += val2;
                /*  Add digit 1 or 2 → state 0  */
                nf0[kk] += val2 * (seq_t)2;
            }
        }
        /*  Swap arrays for next iteration  */
        seq_t *tmp;
        tmp = f0; f0 = nf0; nf0 = tmp;
        tmp = f1; f1 = nf1; nf1 = tmp;
        tmp = f2; f2 = nf2; nf2 = tmp;
    }

    /*  Merge states to obtain T(r,k)  */
    seq_t result = (seq_t)0;
    if (k <= maxK) {
        result = f0[k] + f1[k] + f2[k];
    }

    /*  Free memory  */
    mem_free(f0); mem_free(f1); mem_free(f2);
    mem_free(nf0); mem_free(nf1); mem_free(nf2);

    return result;
}
