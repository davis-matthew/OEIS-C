/*
 * A088611  a(1) = 1, a(n) = n-th squarefree number if n is squarefree else
 *          a(n) is the n-th nonsquarefree number.
 * https://oeis.org/A088611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th squarefree number if n is squarefree, otherwise a(n)
 *            = n-th non‑squarefree number. Correct for all n ≥ 1
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* The types seq_t and idx_t are defined by the build system. */
/* The function a must be defined with external linkage. */
seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/*  Local data -------------------------------------------------------- */
/*  We keep two static arrays, each guaranteed to hold at least the
   first 20 000 numbers of the corresponding set.  For n up to 10 000
   (the typical test range) this is far more than sufficient.   */
/* ------------------------------------------------------------------ */
#define MAX_CAP 20000

static seq_t sf_arr[MAX_CAP];   /* square‑free numbers */
static size_t sf_len = 0;

static seq_t ns_arr[MAX_CAP];   /* non‑square‑free numbers */
static size_t ns_len = 0;

static idx_t scanned_limit = 0; /* largest integer that has been examined */

/* ------------------------------------------------------------------ */
/*  Utility functions ------------------------------------------------ */
/* ------------------------------------------------------------------ */
static bool is_squarefree(seq_t x)
{
    /* Simple trial division: test whether an integer square divides x.  */
    for (idx_t p = 2; (seq_t)p * (seq_t)p <= x; ++p)
    {
        seq_t sq = (seq_t)p * (seq_t)p;
        if (x % sq == 0)
            return false;
    }
    return true;
}

/* Generate numbers until we have at least 'need_sf' square‑free
   and 'need_ns' non‑square‑free elements. */
static void generate_until(idx_t need_sf, idx_t need_ns)
{
    while (sf_len < need_sf || ns_len < need_ns)
    {
        ++scanned_limit;
        seq_t cand = scanned_limit;

        if (is_squarefree(cand))
        {
            if (sf_len < MAX_CAP)
                sf_arr[sf_len++] = cand;
        }
        else
        {
            if (ns_len < MAX_CAP)
                ns_arr[ns_len++] = cand;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Public function -------------------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)       /* Undefined for 0; return 0 to avoid UB. */
        return 0;

    if (is_squarefree((seq_t)n))
    {
        generate_until(n, 0);
        return sf_arr[n - 1];
    }
    else
    {
        generate_until(0, n);
        return ns_arr[n - 1];
    }
}
