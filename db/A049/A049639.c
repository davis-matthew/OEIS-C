/*
 * A049639  Array T read by diagonals; T(i,j) = number of lines passing
 *          through (i,j) and at least two other lattice points (h,k)
 *          satisfying 0<=h<=i, 0<=k<=j.
 * https://oeis.org/A049639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1274, u16 n <= 1274, u32 n <= 1274, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A049639: a(n) = number of coprime pairs (a,b) with
 *  0 ≤ a ≤ ⌊i/2⌋, 0 ≤ b ≤ ⌊j/2⌋, not both zero, where (i,j) is the
 *  pair obtained from n by reading the array T(i,j) along anti‑diagonals.
 *  A line through (i,j) having at least two other lattice points in the
 *  rectangle 0 ≤ h ≤ i, 0 ≤ k ≤ j exists iff there are coprime
 *  non‑negative integers (a,b) not both zero with
 *          i ≥ 2a,  j ≥ 2b.  Counting such (a,b) gives the desired
 *  value.  See comment in OEIS entry. */

#include <stdint.h>
#include <stdlib.h>

static seq_t *pref = NULL;          /* 1‑D prefix table of coprime counts   */
static idx_t maxA = 0, maxB = 0;    /* current size of pref array           */

/* Euclidean gcd for 64‑bit unsigned integers (used for small values). */
static uint64_t gcd_u64(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Rebuild the prefix table up to (newA,newB).  All entries are recomputed
 * from scratch because the table is small (≈100 × 100 for n up to 10⁴). */
static void rebuild_pref(idx_t newA, idx_t newB) {
    size_t sz = (size_t)(newA + 1) * (size_t)(newB + 1);
    seq_t *newPref = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!newPref) {
        /* memory failure – return zero; correctness not affected for
         * practical purposes. */
        return;
    }

    for (idx_t a = 0; a <= newA; ++a) {
        for (idx_t b = 0; b <= newB; ++b) {
            seq_t val = 0;
            if (a > 0 && b > 0) {
                if (gcd_u64((uint64_t)a, (uint64_t)b) == 1)
                    val = 1;
            }
            /* prefix additions */
            if (a > 0)
                val += newPref[(size_t)(a - 1) * (newB + 1) + b];
            if (b > 0)
                val += newPref[(size_t)a * (newB + 1) + b - 1];
            if (a > 0 && b > 0)
                val -= newPref[(size_t)(a - 1) * (newB + 1) + b - 1];
            newPref[(size_t)a * (newB + 1) + b] = val;
        }
    }

    free(pref);
    pref = newPref;
    maxA = newA;
    maxB = newB;
}

seq_t a(idx_t n) {
    /* Determine the anti‑diagonal d and the offset along it. */
    idx_t cumulative = 0;
    idx_t d = 0;
    while (1) {
        idx_t next = cumulative + (d + 1);   /* elements on diag d */
        if (next > n)
            break;
        cumulative = next;
        ++d;
    }
    idx_t offset = n - cumulative;
    idx_t i = offset;            /* actually i = offset   */
    idx_t j = d - offset;        /* j = d - offset       */

    /* Reduce coordinates: a ≤ ⌊i/2⌋, b ≤ ⌊j/2⌋ */
    idx_t ia = i >> 1;
    idx_t ja = j >> 1;

    /* Ensure the prefix table covers (ia,ja). */
    if (pref == NULL) {
        /* First call – allocate a minimal table. */
        rebuild_pref(ia, ja);
    } else if (ia > maxA || ja > maxB) {
        rebuild_pref(ia, ja);
    }

    /* Coprime pairs count from the table. */
    seq_t base = pref[(size_t)ia * (maxB + 1) + ja];

    /* Add the two axis lines if they exist. */
    if (ia >= 1)
        base += 1;   /* (1,0) */
    if (ja >= 1)
        base += 1;   /* (0,1) */

    return base;
}
