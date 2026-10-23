/*
 * A171859  a(n) = Bell(n) - Sum_{j=0..n-1} Bell(j), where the Bell numbers
 *          are given in A000110.
 * https://oeis.org/A171859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(n) - Sum_{j=0..n-1} Bell(j)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cached arrays and state */
static seq_t *Bell    = NULL;
static seq_t *sumBell = NULL;
static seq_t *stir    = NULL;
static size_t bellCap      = 0;
static size_t sumBellCap   = 0;
static size_t stirCap      = 0;
static idx_t  maxComputed  = (idx_t)-1;   /* no values computed yet */

/* Ensure all arrays are large enough for indices up to n */
static void ensure_capacity(idx_t n)
{
    size_t needBell = (size_t)(n + 1);              /* Bell[0..n]   */
    if (Bell == NULL || bellCap < needBell) {
        Bell = (seq_t *)realloc(Bell, needBell * sizeof(seq_t));
        bellCap = needBell;
    }
    size_t needStir = needBell;                    /* stir[0..n]   */
    if (stir == NULL || stirCap < needStir) {
        stir = (seq_t *)realloc(stir, needStir * sizeof(seq_t));
        stirCap = needStir;
    }
    size_t needSumBell = (size_t)(n + 2);          /* sumBell[0..n+1] */
    if (sumBell == NULL || sumBellCap < needSumBell) {
        sumBell = (seq_t *)realloc(sumBell, needSumBell * sizeof(seq_t));
        sumBellCap = needSumBell;
    }
}

/* Compute all Bell and prefix‑sum values up to index n */
static void compute_up_to(idx_t n)
{
    if (maxComputed != (idx_t)-1 && maxComputed >= n)
        return;                     /* already computed */

    ensure_capacity(n);

    /* Initialisation for n = 0 (performed once) */
    if (maxComputed == (idx_t)-1) {
        Bell[0]   = (seq_t)1;
        sumBell[0]= (seq_t)0;
        sumBell[1]= Bell[0];         /* prefix sum after Bell[0] */
        stir[0]   = (seq_t)1;         /* S(0,0)=1     */
        for (idx_t i = 1; i <= n; ++i) 
            stir[i] = (seq_t)0;       /* other entries start at 0 */
        maxComputed = 0;
    }

    /* Iterate to compute further values */
    for (idx_t cur = maxComputed + 1; cur <= n; ++cur) {
        /* Stirling numbers of the second kind:
           S(cur,k) = k * S(cur-1,k) + S(cur-1,k-1) */
        for (idx_t k = cur; k >= 1; --k) {
            stir[k] = stir[k] * (seq_t)k + stir[k-1];
        }
        stir[0] = (seq_t)0;

        /* Bell(cur) = sum_{k=1..cur} S(cur,k) */
        seq_t bellCur = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k) {
            bellCur += stir[k];
        }
        Bell[cur] = bellCur;

        /* Prefix sum of Bell numbers:
           sumBell[i] = sum_{j=0}^{i-1} Bell[j] */
        sumBell[cur+1] = sumBell[cur] + bellCur;

        maxComputed = cur;
    }
}

/* Required external function */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return Bell[n] - sumBell[n];
}
