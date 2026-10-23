/*
 * A026358  a(n) = a(m) if a(m) has already occurred exactly once and n =
 *          a(m)+2m-2, else a(n) = least positive integer that has not yet
 *          occurred.
 * https://oeis.org/A026358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
    OEIS A026358
    a(n) = a(m) if a(m) has occurred exactly once and n = a(m)+2m-2
    otherwise a(n) = least positive integer that has not yet occurred.
    Offset: 1
*/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ----- internal data structures -------------------------------------- */
struct Dist {
    seq_t   val;   /* distinct value               */
    idx_t   count; /* number of occurrences so far */
};

/* ----- static state --------------------------------------------------- */
static seq_t  *seq      = NULL;          /* 1‑based sequence values      */
static size_t  seq_cap  = 0;             /* allocated sequence capacity  */
static idx_t   current_n = 0;            /* highest n computed           */

static struct Dist *dist    = NULL;      /* array of distinct values    */
static size_t       dist_cap = 0;        /* allocated capacity for dist  */
static size_t       dist_len = 0;        /* number of distinct values   */

static bool  *seen_small = NULL;        /* 0‑based flag if small int seen */
static size_t seen_cap   = 0;            /* allocated length for seen_small */

/* ----- helper prototypes --------------------------------------------- */
static void ensure_seq_cap(idx_t n);
static void ensure_dist_cap(void);
static void ensure_seen_cap(idx_t idx);
static idx_t  get_dist_count(const seq_t *v);
static void   inc_dist_count(const seq_t *v);
static void  update_next_missing(const seq_t *v, idx_t *next_missing);

/* ----- entry function ------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise on first call ---------------------------------------- */
    if (!seq) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        /* seq[0] is unused; entries will be set when computed       */

        dist = NULL;
        dist_cap = 0;
        dist_len = 0;

        seen_small = NULL;
        seen_cap   = 0;
    }

    /* compute up to required index ------------------------------------- */
    if (n > current_n) {
        /* increase sequence array if necessary */
        ensure_seq_cap(n + 2);              /* allocate n+1 slots (1‑based) */

        /* keep track of the smallest positive integer not yet seen */
        static idx_t next_missing = 1;

        /* compute terms current_n+1 .. n */
        for (idx_t i = current_n + 1; i <= n; ++i) {
            seq_t val = 0;
            bool found = false;

            /* search all previous m for the defining condition */
            for (idx_t m = 1; m < i; ++m) {
                seq_t candidate = seq[m];
                seq_t comp = candidate + ((seq_t)2 * m - 2);
                if (comp == (seq_t)i && get_dist_count(&candidate) == 1) {
                    val = candidate;
                    found = true;
                    break;
                }
            }

            /* if none found, use the least unused positive integer */
            if (!found)
                val = (seq_t)next_missing;

            /* store the new value */
            seq[i] = val;

            /* update counts of distinct values */
            inc_dist_count(&val);

            /* update next_missing if we just used it */
            if (val == (seq_t)next_missing)
                update_next_missing(&val, &next_missing);
        }

        current_n = n;
    }

    return seq[n];
}

/* ----- helper implementations ---------------------------------------- */
static void ensure_seq_cap(idx_t n)
{
    if (n > seq_cap) {
        size_t new_cap = seq_cap * 2;
        if (new_cap < (size_t)n) new_cap = n;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

static void ensure_dist_cap(void)
{
    if (dist_len == dist_cap) {
        size_t new_cap = dist_cap ? dist_cap * 2 : 64;
        dist = (struct Dist *)realloc(dist, new_cap * sizeof(struct Dist));
        dist_cap = new_cap;
    }
}

static void ensure_seen_cap(idx_t idx)
{
    if (idx >= seen_cap) {
        size_t new_cap = seen_cap ? seen_cap * 2 : 64;
        while (new_cap <= (size_t)idx) new_cap *= 2;
        seen_small = (bool *)realloc(seen_small, new_cap * sizeof(bool));
        memset(seen_small + seen_cap, 0, (new_cap - seen_cap) * sizeof(bool));
        seen_cap = new_cap;
    }
}

static idx_t get_dist_count(const seq_t *v)
{
    for (size_t i = 0; i < dist_len; ++i)
        if (dist[i].val == *v)
            return dist[i].count;
    return 0;
}

static void inc_dist_count(const seq_t *v)
{
    for (size_t i = 0; i < dist_len; ++i) {
        if (dist[i].val == *v) {
            ++dist[i].count;
            return;
        }
    }
    /* not found – add new entry */
    ensure_dist_cap();
    dist[dist_len].val = *v;
    dist[dist_len].count = 1;
    ++dist_len;

    /* if the value is a small integer, mark it as seen */
    if (*v <= (seq_t)(seen_cap - 1)) {
        ensure_seen_cap((idx_t)*v);
        seen_small[(idx_t)*v] = true;
    }
}

static void update_next_missing(const seq_t *v, idx_t *next_missing)
{
    /* val equal to current next_missing has just been seen.
       Advance to the next missing positive integer.   */
    do {
        ++(*next_missing);
        if (*next_missing >= seen_cap)
            ensure_seen_cap(*next_missing);
    } while (seen_small[*next_missing]);
}
