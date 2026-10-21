/*
 * A097569  Right-truncatable Harshad numbers (zeros not permitted).
 * https://oeis.org/A097569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   a(n) = n-th smallest right‑truncatable Harshad number * (Harshad
 *            number in which every right truncation is also a Harshad). * The
 *            algorithm builds all such numbers once by breadth‑first *
 *            generation from the digits 1…9, stores them sorted, and returns
 *            the * requested term. This works for any buil...
 * Range:     u8 n <= 16, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Configuration – enough space for all terms (the sequence is
 * finite with last term 24786).  Increase if a larger build type
 * is used. */
#define MAX_TERMS  10000
#define MAX_NODES  10000

/* ------------------------------------------------------------------ */
struct node
{
    seq_t val;   /* number itself     */
    seq_t sum;   /* sum of decimal digits */
};

/* ------------------------------------------------------------------ */
/* Cache of all sequence terms (sorted after construction).         */
static seq_t  cache[MAX_TERMS];
static size_t cache_len = 0;

/* Two queues used for breadth‑first expansion.                     */
static struct node  nodes_a[MAX_NODES];
static struct node  nodes_b[MAX_NODES];

/* Flag whether the cache has been built.                          */
static bool built = false;

/* ------------------------------------------------------------------ */
/* Comparison routine for qsort over seq_t.                          */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/* ------------------------------------------------------------------ */
/* Append a value to the cache – assumes we have room.               */
static void add_cache(seq_t v)
{
    if (cache_len < MAX_TERMS)
        cache[cache_len++] = v;
}

/* ------------------------------------------------------------------ */
/* Build the cache of all right‑truncatable Harshad numbers.         */
static void build_cache(void)
{
    struct node *cur  = nodes_a;
    struct node *next = nodes_b;
    size_t cur_len = 0, next_len = 0;

    /* Initialise with one‑digit Harshads 1…9. */
    for (seq_t d = 1; d <= 9; ++d)
    {
        cur[cur_len++] = (struct node){ d, d };
        add_cache(d);
    }

    /* Breadth‑first expansion. */
    while (cur_len > 0)
    {
        for (size_t i = 0; i < cur_len; ++i)
        {
            seq_t v = cur[i].val;
            seq_t s = cur[i].sum;
            for (seq_t d = 1; d <= 9; ++d)
            {
                seq_t nv = v * 10 + d;
                seq_t ns = s + d;
                if (nv % ns == 0)
                {
                    add_cache(nv);
                    next[next_len++] = (struct node){ nv, ns };
                }
            }
        }
        /* Prepare next level. */
        struct node *tmp = cur; cur = next; next = tmp;
        cur_len = next_len; next_len = 0;
    }

    /* Sort the cache.  The size is at most MAX_TERMS. */
    qsort(cache, cache_len, sizeof(seq_t), seq_cmp);
    built = true;
}

/* ------------------------------------------------------------------ */
/* The required sequence function.                                  */
seq_t a(idx_t n)
{
    if (!built) build_cache();
    if (n == 0 || n > cache_len) return 0;   /* out of bounds */
    return cache[n - 1];
}
