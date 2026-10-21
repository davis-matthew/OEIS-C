/*
 * A052783  Expansion of e.g.f. x*log(-1/(-1+x))^5.
 * https://oeis.org/A052783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A052783: Expansion of e.g.f. x*log(-1/(-1+x))^5
   Formula: a(n) = 120 * n * s(n-1,5)
   where s(n,k) are the unsigned Stirling numbers of the first kind. */
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Helper: compute unsigned Stirling numbers of the first kind
    s(n,k) for k ≤ 5.  Stored in a flat array s[n][0..5]. */
/* ------------------------------------------------------------------ */
static seq_t *s = NULL;          /* flat array of Stirling numbers */
static idx_t  curN = 0;          /* largest n for which s is filled (0 ≤ n ≤ curN) */

/* Ensure that s[i][k] is available for all 0 ≤ i ≤ max_n and 0 ≤ k ≤ 5. */
static void ensure_stirling(idx_t max_n)
{
    if (curN >= max_n) return;  /* already sufficient */

    /* Allocate or enlarge the table.  Size = (max_n+1) rows × 6 columns. */
    idx_t newN = max_n;
    seq_t *newS = (seq_t *)realloc(s, (newN + 1) * 6 * sizeof(seq_t));
    if (!newS) abort();          /* out of memory – crash in a sane build */
    s = newS;

    /* Zero‐initialize new rows and columns. */
    for (idx_t i = curN + 1; i <= newN; ++i)
        for (int j = 0; j < 6; ++j)
            s[i * 6 + j] = 0;

    /* Base case: s(0,0)=1.  If curN==0 we just set the first element. */
    if (curN == 0)
        s[0] = 1;

    /* Compute rows curN+1 .. newN using recurrence:
       s(n,k) = s(n-1,k-1) + (n-1)*s(n-1,k)   for 1≤k≤5. */
    for (idx_t i = curN + 1; i <= newN; ++i) {
        for (int k = 1; k <= 5; ++k) {
            seq_t left  = s[(i - 1) * 6 + (k - 1)];   /* s(i-1,k-1) */
            seq_t right = s[(i - 1) * 6 + k] * (seq_t)(i - 1); /* (i-1)*s(i-1,k) */
            s[i * 6 + k] = left + right;
        }
    }

    curN = newN;
}

/* ------------------------------------------------------------------ */
/*  Public interface as required by the build system. */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 6)
        return 0;            /* a(0) … a(5) are zero */

    /* Ensure we have s(n-1,5). */
    ensure_stirling(n - 1);

    seq_t val = s[(n - 1) * 6 + 5];   /* s(n-1,5) */
    return (seq_t)120 * (seq_t)n * val;
}
