/*
 * A037257  a() = 1,3,... [ A037257 ], differences = 2,... [ A037258 ] and 2nd
 *          differences [ A037259 ] are disjoint and monotonic; adjoin next
 *          free number to 2nd differences unless it would produce a duplicate
 *          in which case ignore.
 * https://oeis.org/A037257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 65, u32 n <= 2904, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A037257.
 *
 * Sequence A037257 is built as follows.
 * Start with the two terms a(0)=1 and a(1)=3.
 * For each subsequent term:
 *   • Let dᵢ = a(i)-a(i‑1) be the last first difference.
 *   • Choose the smallest positive integer δ that is not present
 *     in any of the three sets {a(k)}, {d(k)} or {Δd(k)} and
 *     also ensures that dᵢ+δ is not already present in those sets.
 *   • Append δ to the second‑difference set, update dᵢ←dᵢ+δ
 *     and a(i)←a(i‑1)+dᵢ.
 * The resulting sequence begins  
 *   1,3,9,20,38,64,100,148,209,284,374,480,603,745,908,1093,1301,1533,1790,2075,2389,2733,3108,3515,3955,...
 *
 * The implementation memoises the state so that calls can arrive
 * in arbitrary order.  It uses binary search to test membership
 * in the three growing strictly increasing sets.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if val occurs in arr[0 .. len-1] (arr is sorted). */
static inline bool member(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        seq_t v = arr[mid];
        if (v == val) return true;
        if (v < val)     lo = mid + 1;
        else             hi = mid;
    }
    return false;
}

/* Find the smallest number ≥ start that can be used as a second
   difference.  start is the smallest integer candidate to test.
   prev_d is the then‑current last first difference.  The function
   guarantees that the number is not in any of the three sets
   and that prev_d + number is also not in any of the three sets. */
static inline seq_t find_next_delta(seq_t start, seq_t prev_d,
                                    const seq_t *A, size_t lenA,
                                    const seq_t *D1, size_t lenD1,
                                    const seq_t *DD, size_t lenDD)
{
    for (seq_t cand = start; ; ++cand) {
        if (!member(A,  lenA,      cand) &&
            !member(D1, lenD1,    cand) &&
            !member(DD, lenDD,    cand)) {
            seq_t new_d = prev_d + cand;
            if (!member(A,  lenA,      new_d) &&
                !member(D1, lenD1,    new_d) &&
                !member(DD, lenDD,    new_d)) {
                return cand;
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* Static state that survives across calls. */
    static seq_t *A   = NULL, *D1 = NULL, *DD = NULL;
    static size_t capA = 0, capD1 = 0, capDD = 0;
    static size_t lenA = 0, lenD1 = 0, lenDD = 0;
    static seq_t next_start = 1;      /* first integer to test as δ. */
    static bool init = false;

    if (!init) {                      /* one‑time initialisation */
        /* Allocate initial capacities. */
        capA   = 8;  A   = (seq_t *)malloc(capA * sizeof(seq_t));
        capD1  = 8;  D1  = (seq_t *)malloc(capD1 * sizeof(seq_t));
        capDD  = 8;  DD  = (seq_t *)malloc(capDD * sizeof(seq_t));

        /* Set the first two terms a(0)=1, a(1)=3. */
        A[0] = 1;  A[1] = 3;  lenA = 2;

        /* First difference: a(1)-a(0) = 2. */
        D1[0] = 3 - 1;
        lenD1 = 1;

        /* No second differences yet. */
        lenDD = 0;

        init = true;
    }

    /* Generate terms until we have at least n+1 terms. */
    while (lenA <= (size_t)n) {
        seq_t prev_d = D1[lenD1 - 1];

        /* Find the next admissible second difference δ. */
        seq_t delta = find_next_delta(next_start, prev_d, A, lenA, D1, lenD1, DD, lenDD);

        /* Compute new first difference and new term. */
        seq_t new_d = prev_d + delta;
        seq_t new_a = A[lenA - 1] + new_d;

        /* Append the new items to the three arrays. */
        /* Grow arrays if necessary. */
        if (lenD1 == capD1) {
            capD1 = capD1 ? capD1 * 2 : 8;
            D1 = (seq_t *)realloc(D1, capD1 * sizeof(seq_t));
        }
        D1[lenD1++] = new_d;

        if (lenA == capA) {
            capA = capA ? capA * 2 : 8;
            A = (seq_t *)realloc(A, capA * sizeof(seq_t));
        }
        A[lenA++] = new_a;

        if (lenDD == capDD) {
            capDD = capDD ? capDD * 2 : 8;
            DD = (seq_t *)realloc(DD, capDD * sizeof(seq_t));
        }
        DD[lenDD++] = delta;

        /* Next search will start just after the used δ. */
        next_start = delta + 1;
    }

    return A[(size_t)n];
}
