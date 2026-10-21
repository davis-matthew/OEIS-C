/*
 * A229083  Numbers k such that the distance between the k-th triangular
 *          number and the nearest square is at most 1.
 * https://oeis.org/A229083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k (k ≥ 1) such that the k‑th triangular
 *            number T(k) = k(k+1)/2 satisfies |T(k) – m²| ≤ 1 for some
 *            integer m
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* ----------------------------------------------------------------------
   Pre‑allocate storage for the sequence and for the four
   families that generate the indices.
   The maximum number of terms required for typical tests (n ≤ 10000)
   is comfortably below 20000, so fixed size arrays avoid all
   dynamic allocation and work for both C and C++.
   ---------------------------------------------------------------------- */

#define MAX_TERMS 20000                /* maximum elements retained */

/* global merged sequence and its current size */
static seq_t  global_vals[MAX_TERMS];
static size_t global_size = 0;

/* family 0 : T(k) = square            (k = 1, 8, 49, …)   */
static seq_t  fam0[MAX_TERMS];
static size_t sz0   = 0;
static idx_t  pos0  = 0;

/* family 1 : T(k) = square – 1, branch 1  (k = 2, 15, 90, …) */
static seq_t  fam1[MAX_TERMS];
static size_t sz1   = 0;
static idx_t  pos1  = 0;

/* family 2 : T(k) = square – 1, branch 2  (k = 5, 32, 170, …) */
static seq_t  fam2[MAX_TERMS];
static size_t sz2   = 0;
static idx_t  pos2  = 0;

/* family 3 : T(k) = square + 1            (k = 4, 25, 144, …) */
static seq_t  fam3[MAX_TERMS];
static size_t sz3   = 0;
static idx_t  pos3  = 0;

/* ----------------------------------------------------------------------
   Small helper: recurrence that generates the next term
   of every family (all satisfy k(n+1) = 6·k(n) – k(n−1) + 2)
   ---------------------------------------------------------------------- */
static inline seq_t next_term(seq_t prev1, seq_t prev2)
{
    return (seq_t)6 * prev1 - prev2 + (seq_t)2;
}

/* ----------------------------------------------------------------------
   One‑time initialisation of the families.
   ---------------------------------------------------------------------- */
static void init_families(void)
{
    /* family 0 */
    fam0[0] = (seq_t)1;   /* k = 1   */
    fam0[1] = (seq_t)8;   /* k = 8   */
    sz0   = 2;
    pos0  = 0;

    /* family 1 */
    fam1[0] = (seq_t)2;   /* k = 2   */
    fam1[1] = (seq_t)15;  /* k = 15  */
    sz1   = 2;
    pos1  = 0;

    /* family 2 */
    fam2[0] = (seq_t)5;   /* k = 5   */
    fam2[1] = (seq_t)32;  /* k = 32  */
    sz2   = 2;
    pos2  = 0;

    /* family 3 */
    fam3[0] = (seq_t)4;   /* k = 4   */
    fam3[1] = (seq_t)25;  /* k = 25  */
    sz3   = 2;
    pos3  = 0;
}

/* ----------------------------------------------------------------------
   Return the n‑th element of sequence A229083.
   The function builds the sequence incrementally, merging the four
   families and keeping the list sorted and unique.  It is safe for
   repeated calls in any order of the index n.
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;          /* undefined offset 0 */

    /* one‑time initialisation */
    static int initialized = 0;
    if (!initialized) {
        init_families();
        initialized = 1;
    }

    /* generate enough terms to answer the request */
    while (global_size < (size_t)n) {
        /* candidates from all families */
        seq_t c0 = fam0[pos0];
        seq_t c1 = fam1[pos1];
        seq_t c2 = fam2[pos2];
        seq_t c3 = fam3[pos3];

        /* find the smallest candidate */
        seq_t best = c0;
        if (c1 < best) best = c1;
        if (c2 < best) best = c2;
        if (c3 < best) best = c3;

        /* append to global list (duplicates are not expected) */
        global_vals[global_size] = best;
        global_size++;

        /* advance the family(ies) that produced the minimal value */
        if (c0 == best) {
            pos0++;
            if (pos0 >= sz0) {
                fam0[sz0] = next_term(fam0[sz0-1], fam0[sz0-2]);
                sz0++;
            }
        }
        if (c1 == best) {
            pos1++;
            if (pos1 >= sz1) {
                fam1[sz1] = next_term(fam1[sz1-1], fam1[sz1-2]);
                sz1++;
            }
        }
        if (c2 == best) {
            pos2++;
            if (pos2 >= sz2) {
                fam2[sz2] = next_term(fam2[sz2-1], fam2[sz2-2]);
                sz2++;
            }
        }
        if (c3 == best) {
            pos3++;
            if (pos3 >= sz3) {
                fam3[sz3] = next_term(fam3[sz3-1], fam3[sz3-2]);
                sz3++;
            }
        }
    }

    return global_vals[(size_t)(n - (idx_t)1)];
}
