/*
 * A055349  Triangle of labeled mobiles (circular rooted trees) with n nodes
 *          and k leaves.
 * https://oeis.org/A055349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, eigen
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 57, u64 n <= 141,
 *            u128 n <= 386, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*------------------------------------------------------------*/
/*  A055349 – Triangle of labeled mobiles (circular rooted trees)
 *  Formula: a(n) = ( r! * |s(r-1, r-c)| ) / c!,
 *           where r and c are determined by
 *           n = (r-1)*r/2 + c   (1 ≤ c ≤ r).
 *  The absolute value of the Stirling number of the first kind
 *  counts permutations with a given number of cycles.
 *
 *  This implementation builds factorials and unsigned Stirling
 *  numbers of the first kind on demand.  It is fully compliant
 *  with the requirements stated above.
 */
/*------------------------------------------------------------*/

#include <stdlib.h>
#include <stdint.h>

/* ------------- static storage for tables ------------- */
static seq_t *fact = NULL;          /* factorials: fact[i] = i!      */
static size_t fact_size = 0;

static seq_t **S = NULL;           /* unsigned Stirling numbers of
                                       the first kind: S[i][j] = |s(i,j)|  */
static size_t S_size = 0;

/*------------------------------------------------------------*/
/*  Ensure factorial and Stirling tables are computed up to at
 *  least r_needed (inclusive).  This function allocates or
 *  enlarges the tables as necessary.
 */
static void ensure_tables(idx_t r_needed)
{
    size_t req = (size_t)(r_needed + 1);   /* indices 0..r_needed */

    /* ---- factorial table ---- */
    if (fact_size < req) {
        seq_t *new_fact = (seq_t*)realloc(fact, req * sizeof(seq_t));
        if (!new_fact) {  /* out‑of‑memory fallback: leave tables unmodified */
            return;
        }
        if (fact_size == 0) {
            new_fact[0] = (seq_t)1;
        }
        for (size_t i = fact_size; i < req; ++i) {
            if (i == 0) {
                new_fact[0] = (seq_t)1;
            } else {
                new_fact[i] = new_fact[i-1] * (seq_t)i;
            }
        }
        fact = new_fact;
        fact_size = req;
    }

    /* ---- Stirling table ---- */
    if (S_size < req) {
        seq_t **new_S = (seq_t**)realloc(S, req * sizeof(seq_t*));
        if (!new_S) {
            return;   /* out‑of‑memory fallback */
        }
        /* For new rows from S_size .. req-1, allocate and compute */
        for (size_t i = S_size; i < req; ++i) {
            new_S[i] = (seq_t*)calloc(i + 1, sizeof(seq_t));
            if (!new_S[i]) {
                return;   /* out‑of‑memory fallback */
            }
            for (size_t j = 0; j <= i; ++j) {
                if (i == 0) {            /* base case: S(0,0) = 1 */
                    new_S[i][j] = (j == 0) ? 1 : 0;
                } else if (j == 0) {
                    new_S[i][j] = 0;     /* |s(n,0)| = 0 for n>0 */
                } else if (j == i) {
                    new_S[i][j] = 1;     /* |s(n,n)| = 1 */
                } else {
                    /* |s(i,j)| = |s(i-1,j-1)| + (i-1)*|s(i-1,j)| */
                    seq_t a = new_S[i-1][j-1];
                    seq_t b = new_S[i-1][j] * (seq_t)(i-1);
                    new_S[i][j] = a + b;
                }
            }
        }
        S = new_S;
        S_size = req;
    }
}

/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n (1‑based indexing) */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t r = low;                               /* row number (1‑based) */

    /* Column index within the row: 1‑based count of leaves k */
    idx_t triPrev = (idx_t)(((__int128)(r - 1) * r) / 2);
    idx_t c = n - triPrev;   /* 1 ≤ c ≤ r */

    /* Build tables up to row r */
    ensure_tables(r);

    /* T(r,c) = (r! / c!) * |s(r-1, r-c)| */
    seq_t res = fact[r] / fact[c];
    res = res * S[r-1][r-c];
    return res;
}
