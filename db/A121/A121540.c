/*
 * A121540  Increasing sequence: "if n appears a*n+b does not", case a(1)=3,
 *          a=2, b=1.
 * https://oeis.org/A121540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A121540
 *
 * The sequence a(n) is the increasing list of non‑negative integers
 * starting at 3 such that for every term m of the sequence the
 * number 2·m+1 is not a term of the sequence.  The terms are chosen
 * greedily – each new term is the smallest integer not forbidden by
 * any earlier term.  The first few terms are
 *
 *   3, 4, 5, 6, 8, 10, 12, 14, 15, 16, 18, 19, 20, 22, 23, 24,
 *   26, 27, 28, 30, 32, 34, 35, 36, …
 *
 * The implementation maintains two sorted dynamic arrays:
 *   • seq_buf   – the sequence values already generated
 *   • forbid_buf – all numbers of the form 2·m+1 for m∈seq_buf,
 *                  i.e. all forbidden numbers.
 *
 * Memory growth is achieved with exponential doubling; searching and
 * inserting into the forbidden array is done with binary search to
 * keep the cost O(n log n) for the required range (n≤10 000).
 *
 * The function is thread‑unsafe but has static linkage and can be
 * called with any order of n.
 *
 * seq_t and idx_t are supplied by the build; no external libraries
 * other than <stdlib.h> are used.  All required operations on
 * seq_t are performed via the standard arithmetic operators.
 *
 * The code also works for arbitrary‑precision seq_t because the
 * standard operators (+, -, *, <, ==) are assumed to be defined.
 */

#include <stdlib.h>   /* malloc, realloc, exit, NULL, size_t */

/* static state ----------------------------------------------------- */

static seq_t *seq_buf = NULL;      /* generated sequence values          */
static size_t seq_len = 0;          /* number of stored terms             */
static size_t seq_cap = 0;          /* allocated capacity for seq_buf     */
static seq_t next_candidate = (seq_t)3;   /* next integer to test            */

static seq_t *forbid_buf = NULL;   /* sorted forbidden numbers           */
static size_t forbid_len = 0;       /* number of forbidden values         */
static size_t forbid_cap = 0;       /* allocated capacity for forbid_buf  */

/* ------------------------------------------------------------------ */
/* utility: ensure capacity for seq_buf ------------------------------ */

static void ensure_seq_capacity(size_t need)
{
    if (seq_cap >= need)
        return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) /* allocation failed */
    {
        /* In a production build we would handle this more gracefully. */
        exit(1);
    }
    seq_buf = tmp;
    seq_cap = new_cap;
}

/* utility: ensure capacity for forbid_buf --------------------------- */

static void ensure_forbid_capacity(size_t need)
{
    if (forbid_cap >= need)
        return;
    size_t new_cap = forbid_cap ? forbid_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(forbid_buf, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)
    {
        exit(1);
    }
    forbid_buf = tmp;
    forbid_cap = new_cap;
}

/* binary search: true if value is present in forbid_buf[0..len-1] ----- */

static int contains_forbidden(seq_t value)
{
    size_t lo = 0, hi = forbid_len;
    while (lo < hi)
    {
        size_t mid = lo + (hi - lo) / 2;
        seq_t midval = forbid_buf[mid];
        if (midval == value)
            return 1;
        if (midval < value)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;
}

/* insert a new forbidden value while keeping forbid_buf sorted -------- */

static void insert_forbidden(seq_t value)
{
    /* find insertion point */
    size_t lo = 0, hi = forbid_len;
    while (lo < hi)
    {
        size_t mid = lo + (hi - lo) / 2;
        if (forbid_buf[mid] < value)
            lo = mid + 1;
        else
            hi = mid;
    }
    size_t pos = lo;

    ensure_forbid_capacity(forbid_len + 1);
    if (pos < forbid_len)
    {
        /* shift elements to make room */
        memmove(forbid_buf + pos + 1,
                forbid_buf + pos,
                (forbid_len - pos) * sizeof(seq_t));
    }
    forbid_buf[pos] = value;
    forbid_len++;
}

/* ------------------------------------------------------------------ */
/* compute a(n) ------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* generate until we have at least n terms */
    while (seq_len < (size_t)n)
    {
        seq_t cand = next_candidate;
        /* skip forbidden numbers */
        while (contains_forbidden(cand))
            cand = cand + (seq_t)1;   /* cand <==> cand + 1 */

        /* add the allowed candidate to the sequence */
        ensure_seq_capacity(seq_len + 1);
        seq_buf[seq_len++] = cand;

        /* forbid 2*cand + 1 for future terms */
        seq_t forbid_val = cand * (seq_t)2 + (seq_t)1;
        insert_forbidden(forbid_val);

        /* next candidate starts after the one we just used */
        next_candidate = cand + (seq_t)1;
    }

    return seq_buf[(size_t)n - 1];
}
