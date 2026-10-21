/*
 * A060833  Separate the natural numbers into disjoint sets A, B with 1 in A,
 *          such that the sum of any 2 distinct elements of the same set never
 *          equals 2^k + 2. Sequence gives elements of set A.
 * https://oeis.org/A060833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A060833: partition the positive integers into two sets
   A and B such that 1∈A, and no two distinct elements from the same set
   sum to 2^k+2 (k≥0).  The function returns the n‑th element of set A. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT      5000000      /* sufficient for up to ~10⁴ elements of A   */
#define MAX_A_EST  200000       /* maximum number of A‑elements we may need */

static unsigned char inA[LIMIT + 1];      /* membership flag for set A   */
static seq_t        aArray[MAX_A_EST];    /* stored A‑elements           */
static idx_t        aCount  = 0;           /* current size of aArray      */
static idx_t        nextNum = 0;           /* next natural number to test */
static int          initialized = 0;        /* one‑time initialization flag */

/* one‑time initialisation of the inA table */
static void init(void)
{
    if (!initialized)
    {
        memset(inA, 0, sizeof(inA));
        initialized = 1;
    }
}

/* ------------------------------------------------------------------ */
/* Main routine                                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init();                         /* ensure the table is ready */

    /* Generate A-elements until we have at least n of them */
    while (aCount < n)
    {
        ++nextNum;
        seq_t candidate = nextNum;

        int canAdd = 1;              /* assume we can place in A */

        /* For each possible forbidden sum 2^k+2 we check whether
           the other addend already lies in A                           */
        seq_t maxSum = (seq_t)2 * candidate;
        for (seq_t p = 1; p + 2 <= maxSum; p <<= 1)
        {
            seq_t sumForbidden = p + 2;
            seq_t other = sumForbidden - candidate;  /* the possible partner */

            if (other > 0 && other < candidate &&
                inA[(size_t)other])
            {
                canAdd = 0;               /* conflict found */
                break;
            }
        }

        if (canAdd)
        {
            /* guard against accidental overflow of the static array */
            if (aCount >= MAX_A_EST)
                return (seq_t)0;          /* should not happen for requested limits */

            aArray[aCount++] = candidate;
            inA[(size_t)candidate] = 1;  /* mark as belonging to A */
        }
        /* numbers that cannot be added go to set B; nothing else is needed */
    }

    return aArray[n - 1];
}
