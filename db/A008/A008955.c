/*
 * A008955  Triangle of central factorial numbers |t(2n,2n-2k)| read by rows.
 * https://oeis.org/A008955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, nice, easy
 * Formula:   t(m,0) = 1, t(m,m) = (m!)², t(m,k) = m²·t(m‑1,k‑1) + t(m‑1,k)
 *            for 1 ≤ k < m. The sequence a(n) is the flattened triangle of
 *            t(m,k) read row by row (row 0: 1; row 1: 1,1; row 2: 1,5,4; …)
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 50, u64 n <= 101,
 *            u128 n <= 248, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Persistent cache of the triangle entries. */
static seq_t *terms     = NULL;   /* flat array of all terms          */
static size_t termCap   = 0;      /* capacity of 'terms'              */
static size_t termSize  = 0;      /* number of terms stored           */

/* Arrays for the previous and current rows. */
static seq_t *prevRow    = NULL;  /* row r‑1 */
static seq_t *curRow     = NULL;  /* row r   */
static size_t rowCap     = 0;     /* capacity of row arrays           */

/* Highest row that has been fully generated.  A value of (idx_t)-1
   indicates that no rows have been generated yet. */
static idx_t lastRow      = (idx_t)-1;

/* Current factorial value for the latest regenerated row.
   fact(r) = 1·2···r  (with fact(0)=1). */
static seq_t fact          = (seq_t)1;

/* Ensure capacity for at least 'need' terms. */
static void grow_terms (size_t need)
{
    if (need <= termCap)
        return;
    size_t newCap = termCap ? termCap * 2 : 64;
    while (newCap < need)
        newCap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, newCap * sizeof(seq_t));
    if (!tmp)
        abort();
    terms = tmp;
    termCap = newCap;
}

/* Ensure capacity for at least 'len' entries in each row array. */
static void grow_rows (size_t len)
{
    if (len <= rowCap)
        return;
    size_t newCap = rowCap ? rowCap * 2 : 64;
    while (newCap < len)
        newCap *= 2;
    seq_t *tmpPrev = (seq_t *)realloc(prevRow, newCap * sizeof(seq_t));
    seq_t *tmpCur  = (seq_t *)realloc(curRow , newCap * sizeof(seq_t));
    if (!tmpPrev || !tmpCur)
        abort();
    prevRow = tmpPrev;
    curRow  = tmpCur;
    rowCap  = newCap;
}

/* Build the row numbered 'r' (r ≥ 1) and append it to the cache. */
static void build_row (idx_t r)
{
    grow_rows((size_t)r + 1);
    /* First entry is always 1. */
    curRow[0] = (seq_t)1;

    /* Compute middle entries using the recurrence. */
    seq_t rr = (seq_t)r * (seq_t)r;   /* r² */
    for (idx_t k = 1; k < r; ++k)
        curRow[k] = rr * prevRow[k - 1] + prevRow[k];

    /* Compute the final entry as (r!)². */
    fact = fact * (seq_t)r;  /* fact holds r! */
    curRow[r] = fact * fact;

    /* Append the new row to the flat cache. */
    grow_terms(termSize + (size_t)r + 1);
    for (idx_t k = 0; k <= r; ++k)
        terms[termSize++] = curRow[k];

    /* Prepare for the next row. */
    seq_t *tmp = prevRow;
    prevRow    = curRow;
    curRow     = tmp;

    lastRow = r;
}

/* Public entry point.  Generates rows lazily until the n‑th term is
   available. */
seq_t a (idx_t n)
{
    size_t idx = (size_t)n;
    if (idx < termSize)
        return terms[idx];

    /* First call: initialise row 0. */
    if (lastRow == (idx_t)-1)
    {
        grow_terms(1);
        grow_rows(1);
        prevRow[0] = (seq_t)1;
        terms[0]   = (seq_t)1;
        termSize   = 1;
        fact       = (seq_t)1;
        lastRow    = 0;
    }

    while (idx >= termSize)
        build_row(lastRow + 1);

    return terms[idx];
}
