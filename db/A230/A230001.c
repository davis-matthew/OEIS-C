/*
 * A230001  Triangular array read by rows: row n shows the coefficients of the
 *          polynomial v(n) = d(0) + d(1)*x + ... + d(k)*x^k which is the
 *          denominator of the n-th convergent of the continued fraction [1,
 *          1/x, 1/x^2, ... ,1/x^n].
 * https://oeis.org/A230001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A230001 – coefficients of the denominator polynomials
 * v(n) of the continued fraction [1, 1/x, 1/x², …, 1/xⁿ].
 * Recurrence for the denominator polynomials:
 *   v(0) = 1
 *   v(1) = 1
 *   v(n) = v(n-1) + x^(2n-1) · v(n-2)  (n ≥ 2)
 *
 *   a(n) is the n‑th coefficient in the triangular array read by rows:
 *   row r contains the coefficients of v(r) in ascending exponent order.
 *
 * The implementation builds the polynomials incrementally and keeps
 * a cumulative count of terms so that any requested coefficient can
 * be returned in O(1) after the necessary row has been computed.
 * Coefficients are always 0 or 1, so seq_t can be any unsigned integer
 * type or an arbitrary–precision integer class.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"

/* Internal storage for rows of coefficients, their degrees,
 * and cumulative term counts. */
static seq_t **V_ptr   = NULL;   /* V_ptr[n] points to array of deg(v(n))+1
                                  * coefficients (assumed ascending exponent)   */
static size_t *DEGE   = NULL;   /* DEGE[n] = degree of v(n)                      */
static idx_t  *CUM    = NULL;   /* CUM[n]  = total number of terms up to row n  */
static size_t  V_cap  = 0;      /* current capacity of the pointer arrays       */
static idx_t   MAX_COMPUTED = (idx_t)-1; /* largest computed row   */

/* Ensure that the internal arrays can hold at least ‘need’ rows. */
static void ensure_arrays_cap(size_t need)
{
    if (V_cap >= need) return;
    size_t new_cap = V_cap ? V_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    V_ptr = (seq_t **)realloc(V_ptr, new_cap * sizeof(*V_ptr));
    DEGE = (size_t *)realloc(DEGE, new_cap * sizeof(*DEGE));
    CUM  = (idx_t  *)realloc(CUM,  new_cap * sizeof(*CUM));
    V_cap = new_cap;
}

/* Initialise the base rows v(0) and v(1).  This runs only once. */
static void init_base_rows(void)
{
    if (MAX_COMPUTED != (idx_t)-1) return;          /* already initialised */
    ensure_arrays_cap(2);

    /* v(0) = 1  =>  degree 0 */
    V_ptr[0] = seq_alloc_new_array(1);
    V_ptr[0][0] = (seq_t)1;
    DEGE[0] = 0;
    CUM[0]  = 1;                /* number of terms in row 0 */

    /* v(1) = 1  =>  degree 0 */
    V_ptr[1] = seq_alloc_new_array(1);
    V_ptr[1][0] = (seq_t)1;
    DEGE[1] = 0;
    CUM[1]  = CUM[0] + 1;       /* cumulative up to row 1 */

    MAX_COMPUTED = 1;
}

/* Generate all rows up to and including ‘target’ (0‑based indexing). */
static void compute_up_to(idx_t target)
{
    if (target <= MAX_COMPUTED) return;
    ensure_arrays_cap((size_t)target + 1);

    for (idx_t n = MAX_COMPUTED + 1; n <= target; ++n) {
        /* Shift exponent for the contribution from v(n-2). */
        size_t shift = (size_t)(2 * n - 1);
        size_t d1 = DEGE[(size_t)n - 1];
        size_t d2 = DEGE[(size_t)n - 2];
        size_t degn = d1 > d2 + shift ? d1 : d2 + shift;

        V_ptr[n] = seq_alloc_new_array(degn + 1);
        /* Initialise all coefficients to 0. */
        for (size_t i = 0; i <= degn; ++i) V_ptr[n][i] = (seq_t)0;

        /* Add v(n-1) unchanged. */
        for (size_t i = 0; i <= d1; ++i)
            V_ptr[n][i] += V_ptr[(size_t)n - 1][i];

        /* Add x^(2n-1) * v(n-2). */
        for (size_t i = 0; i <= d2; ++i)
            V_ptr[n][i + shift] += V_ptr[(size_t)n - 2][i];

        DEGE[n] = degn;
        CUM[n]  = CUM[(size_t)n - 1] + (idx_t)(degn + 1);
    }
    MAX_COMPUTED = target;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    init_base_rows();               /* make sure rows 0 and 1 exist */

    /* Find the row containing the n‑th coefficient.  CUM[r] holds
       the total number of coefficients up to row r (inclusive). */
    idx_t r = 0;
    while (true) {
        if (r > MAX_COMPUTED) compute_up_to(r);
        if (CUM[r] > n) break;
        ++r;
    }

    idx_t prev = (r > 0) ? CUM[r - 1] : 0;
    size_t pos = (size_t)(n - prev);    /* position within row r */

    /* Guard against out‑of‑bounds (should never happen for valid n). */
    if (pos > DEGE[(size_t)r]) return (seq_t)0;
    return V_ptr[(size_t)r][pos];
}
