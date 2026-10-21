/*
 * A232765  Values of y solving x^2 = floor(y^2/3 + y).
 * https://oeis.org/A232765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A232765:
 *  y-values solving x^2 = floor(y^2/3 + y).
 *  The sequence is the union of three subsequences indexed by n mod 3.
 *  Each subsequence comes from solutions of the Pell equations
 *  s^2 - 12x^2 = 9  (indices 1 mod 3) and
 *  s^2 - 12x^2 = 13 (indices 2 mod 3 or 0 mod 3).
 *  Starting pairs (s,x) are:
 *      A: (3,0)      -> a(1),a(4),...
 *      B1: (5,1)     -> a(2),a(5),...
 *      B2: (11,3)    -> a(3),a(6),...
 *  Recurrence for all:  (s',x') = (7s+24x, 2s+7x)
 *  y = (s-3)/2
 */

#include "../utils/grow_seq.h"

static seq_t *A = NULL, *B1 = NULL, *B2 = NULL;   /* caches for the three subsequences */
static size_t capA = 0, capB1 = 0, capB2 = 0;    /* current buffer capacities   */
static idx_t  cntA = 0, cntB1 = 0, cntB2 = 0;     /* number of elements stored  */

static seq_t sA = 3, xA = 0;   /* pair for subsequence A (N=9)      */
static seq_t sB1 = 5, xB1 = 1; /* pair for subsequence B1 (N=13)    */
static seq_t sB2 = 11, xB2 = 3;/* pair for subsequence B2 (N=13)    */

/* Generate next term of subsequence A (indices 1 mod 3) */
static void ensureA(idx_t k)          /* generate until cntA > k          */
{
    while (cntA <= k) {
        grow_seq(&A, &capA, cntA + 1);
        A[cntA] = (sA - 3) / 2;             /* y = (s-3)/2                  */
        seq_t newS = 7 * sA + 24 * xA;
        seq_t newX = 2 * sA + 7 * xA;
        sA = newS;   xA = newX;
        ++cntA;
    }
}

/* Generate next term of subsequence B1 (indices 2 mod 3) */
static void ensureB1(idx_t k)
{
    while (cntB1 <= k) {
        grow_seq(&B1, &capB1, cntB1 + 1);
        B1[cntB1] = (sB1 - 3) / 2;
        seq_t newS = 7 * sB1 + 24 * xB1;
        seq_t newX = 2 * sB1 + 7 * xB1;
        sB1 = newS;   xB1 = newX;
        ++cntB1;
    }
}

/* Generate next term of subsequence B2 (indices 0 mod 3) */
static void ensureB2(idx_t k)
{
    while (cntB2 <= k) {
        grow_seq(&B2, &capB2, cntB2 + 1);
        B2[cntB2] = (sB2 - 3) / 2;
        seq_t newS = 7 * sB2 + 24 * xB2;
        seq_t newX = 2 * sB2 + 7 * xB2;
        sB2 = newS;   xB2 = newX;
        ++cntB2;
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based; return a(n).  If n % 3 == 1 => subsequence A,
     * n % 3 == 2 => subsequence B1,
     * n % 3 == 0 => subsequence B2.  Use the k‑th element of that
     * subsequence (k = floor((n-1)/3) etc.).                     */
    if (n == 0) return 0;          /* undefined, but harmless            */

    idx_t r = n % 3;
    if (r == 1) {                 /* indices 1,4,7,...  -> A */
        idx_t k = n / 3;
        ensureA(k);
        return A[k];
    } else if (r == 2) {          /* indices 2,5,8,...  -> B1 */
        idx_t k = (n - 2) / 3;
        ensureB1(k);
        return B1[k];
    } else {                      /* indices 3,6,9,...  -> B2 */
        idx_t k = n / 3 - 1;
        ensureB2(k);
        return B2[k];
    }
}
