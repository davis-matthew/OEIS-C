/*
 * A239409  Number of 5Xn 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or two plus the sum of the elements
 *          above it, modulo 4.
 * https://oeis.org/A239409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n 0..3 arrays with no element equal to one
 *            plus the sum of the elements to its left or two plus the sum of
 *            the elements above it, modulo 4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* --- constants for state handling ------------------------------------ */
#define MAX_STATE   1024            /* 4^5 possible row–sum states    */
#define MAX_EDGES   128             /* headroom for transitions per state */

/* --- transition tables ----------------------------------------------- */
static int   edges_len[MAX_STATE];
static unsigned short edges_dest[MAX_STATE][MAX_EDGES];
static unsigned char edges_cnt[MAX_STATE][MAX_EDGES];

/* --- DP working arrays ----------------------------------------------- */
static seq_t *dp_old = NULL;
static seq_t *dp_new = NULL;

/* --- cache of sequence values ---------------------------------------- */
static seq_t *a_cache = NULL;        /* cache[ n ] contains a(n)        */
static idx_t  a_cache_size = 0;      /* allocated size of cache         */
static idx_t  a_cache_computed = 0;   /* highest n that has been computed */
static bool   initialized = false;

/* --- helper: build transition tables --------------------------------- */
static void build_transitions(void)
{
    unsigned char cnt[MAX_STATE];
    unsigned char vi, forb1, forb2;
    int rs[5];
    int tempR, tempV, prefix, next, dest;
    for (int R = 0; R < MAX_STATE; ++R)
    {
        /* decode current row–sum state */
        tempR = R;
        for (int i = 0; i < 5; ++i) { rs[i] = tempR & 3; tempR >>= 2; }

        /* count how many column vectors lead to each resulting state */
        memset(cnt, 0, sizeof(cnt));
        for (int v = 0; v < MAX_STATE; ++v)
        {
            tempV   = v;
            prefix  = 0;
            next    = 0;
            bool ok = true;

            for (int i = 0; i < 5; ++i)
            {
                vi     = tempV & 3;   tempV >>= 2;
                forb1  = (1 + rs[i]) & 3;
                forb2  = (2 + prefix) & 3;
                if (vi == forb1 || vi == forb2) { ok = false; break; }
                prefix = (prefix + vi) & 3;
                dest   = (rs[i] + vi) & 3;
                next  |= dest << (2*i);
            }
            if (!ok) continue;
            cnt[next] += 1;
        }

        /* store non‑zero transitions */
        int len = 0;
        for (int s = 0; s < MAX_STATE; ++s)
        {
            if (cnt[s])
            {
                edges_dest[R][len] = (unsigned short)s;
                edges_cnt[R][len]  = cnt[s];
                ++len;
            }
        }
        edges_len[R] = len;
    }
}

/* --- helper: enlarge cache if necessary ------------------------------- */
static void ensure_cache_size(idx_t needed)
{
    if (needed < a_cache_size) return;
    idx_t new_size = needed + 1;                /* indices 0..needed inclusive */
    a_cache = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
    if (!a_cache) abort();                      /* allocation failure */
    memset(a_cache + a_cache_size, 0,
           (new_size - a_cache_size) * sizeof(seq_t));
    a_cache_size = new_size;
}

/* --------------------------------------------------------------------- */
/*  Main function allowed by the build                                         */
seq_t a(idx_t n)
/*  a(n)  --  a(1) is the first term in the OEIS entry.  */
{
    if (!initialized)
    {
        /* create cache for a up to a few thousand entries */
        ensure_cache_size(10000);     /* enough for the common test range */

        /* prepare DP tables */
        dp_old = (seq_t*)calloc(MAX_STATE, sizeof(seq_t));
        dp_new = (seq_t*)calloc(MAX_STATE, sizeof(seq_t));
        if (!dp_old || !dp_new) abort();

        dp_old[0] = (seq_t)1;         /* 0 columns -> one empty table */
        a_cache_computed = 0;

        /* build transition matrix once */
        build_transitions();

        initialized = true;
    }

    if (n == (idx_t)0) return (seq_t)0;            /* undefined but safe */

    /* extend cache up to the requested n */
    if (n > a_cache_computed)
    {
        for (idx_t step = a_cache_computed + 1; step <= n; ++step)
        {
            /* zero new distribution */
            memset(dp_new, 0, MAX_STATE * sizeof(seq_t));

            /* apply one column transformation */
            for (int R = 0; R < MAX_STATE; ++R)
            {
                seq_t val = dp_old[R];
                if (val == (seq_t)0) continue;

                int len = edges_len[R];
                for (int e = 0; e < len; ++e)
                {
                    int dest = edges_dest[R][e];
                    unsigned char mult = edges_cnt[R][e];
                    dp_new[dest] += val * (seq_t)mult;
                }
            }

            /* swap old/new */
            seq_t *tmp = dp_old;
            dp_old = dp_new;
            dp_new = tmp;

            /* total number of arrays of length = step */
            seq_t total = (seq_t)0;
            for (int i = 0; i < MAX_STATE; ++i) total += dp_old[i];

            ensure_cache_size(step);
            a_cache[step] = total;
        }
        a_cache_computed = n;
    }

    return a_cache[n];
}
