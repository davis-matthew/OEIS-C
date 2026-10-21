/*
 * A075318  Pair the odd numbers such that the k-th pair is (r, r+2k) where r
 *          is the smallest odd number not included earlier:
 *          (1,3),(5,9),(7,13),(11,19),(15,25),(17,29),(21,35),(23,39),(27,45),...
 *          This is the sequence of the second member of pairs.
 * https://oeis.org/A075318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n*phi^2)-1 (phi = (1+sqrt5)/2)
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the pairing construction: the k‑th pair is (r, r+2k)
   where r is the smallest unused odd number.  a(k) is the second member
   (r+2k).  The function is memoised and works for any n ≥ 1. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Static cache of already computed terms */
static seq_t *a_arr = NULL;      /* 1‑indexed array, a_arr[0] unused     */
static idx_t  Nmax  = 0;         /* largest k computed so far           */

/* Boolean table: usedOdd[i] corresponds to odd integer (2*i+1) */
static bool *usedOdd = NULL;
static idx_t usedCap  = 0;       /* current capacity of usedOdd          */
static idx_t nextOddIdx = 0;     /* next candidate odd index             */

/* Ensure that terms up to n are available */
static void ensure_capacity(idx_t n)
{
    if (n <= Nmax)
        return;

    /* ---------- Allocate or grow a_arr ---------- */
    if (a_arr == NULL) {
        a_arr = (seq_t *)calloc(n + 1, sizeof(seq_t));
    } else {
        void *tmp = realloc(a_arr, (n + 1) * sizeof(seq_t));
        a_arr = (seq_t *)tmp;
    }

    /* ---------- Ensure enough space for usedOdd ---------- */
    /* For k terms we need to track odd numbers up to at least
       a(k) ≈ 2.618*k < 3*k, therefore odd indices up to 3*k/2. */
    idx_t requiredCap = (3 * n) / 2 + 10;
    if (requiredCap > usedCap) {
        void *tmp = realloc(usedOdd, requiredCap * sizeof(bool));
        if (usedOdd == NULL)
            memset(tmp, 0, requiredCap * sizeof(bool));
        else
            memset((char *)tmp + usedCap * sizeof(bool),
                   0, (requiredCap - usedCap) * sizeof(bool));
        usedOdd = (bool *)tmp;
        usedCap = requiredCap;
    }

    /* ---------- Generate terms up to n ---------- */
    for (idx_t k = Nmax + 1; k <= n; ++k) {
        /* Find the smallest odd number not yet used. */
        while (nextOddIdx < usedCap && usedOdd[nextOddIdx])
            ++nextOddIdx;

        /* In very rare cases we might be out of allocated space
           (for extremely large n).  Resize if needed. */
        if (nextOddIdx >= usedCap) {
            idx_t newCap = usedCap ? usedCap * 2 : 64;
            while (newCap <= nextOddIdx)
                newCap *= 2;
            void *tmp = realloc(usedOdd, newCap * sizeof(bool));
            memset((char *)tmp + usedCap * sizeof(bool),
                   0, (newCap - usedCap) * sizeof(bool));
            usedOdd = (bool *)tmp;
            usedCap = newCap;
        }

        /* r is the chosen odd number */
        seq_t r = (seq_t)nextOddIdx * 2 + 1;
        seq_t second = r + (seq_t)k * 2;   /* r + 2*k */

        /* Mark r as used */
        usedOdd[nextOddIdx] = true;

        /* Mark the second number as used (it is odd) */
        idx_t secondIdx = (idx_t)((second - 1) / 2);
        if (secondIdx >= usedCap) {
            idx_t newCap = usedCap;
            while (newCap <= secondIdx)
                newCap *= 2;
            void *tmp = realloc(usedOdd, newCap * sizeof(bool));
            memset((char *)tmp + usedCap * sizeof(bool),
                   0, (newCap - usedCap) * sizeof(bool));
            usedOdd = (bool *)tmp;
            usedCap = newCap;
        }
        usedOdd[secondIdx] = true;

        /* Store the term */
        a_arr[k] = second;

        /* Advance to the next candidate odd index for the next round */
        ++nextOddIdx;
    }

    Nmax = n;
}

/* Public function requested by the specification */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for non‑positive indices */

    ensure_capacity(n);
    return a_arr[n];
}
