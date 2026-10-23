/*
 * A130244  Partial sums of the 'upper' Lucas Inverse A130242.
 * https://oeis.org/A130244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A130244: Partial sums of the 'upper' Lucas inverse A130242
   Formula: a(n) = Sum_{k=0..n} A130242(k)
   where A130242(k) = min{ m ≥ 0 | Lucas(m) ≥ k }
   (Lucas(0)=2, Lucas(1)=1, Lucas(m)=Lucas(m-1)+Lucas(m-2).)
   The implementation keeps the partial sums in a cache and
   incrementally builds the Lucas sequence as needed. */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Caches (static state)                                            */
/* ------------------------------------------------------------------ */
static seq_t *a_cache   = NULL;  /* cache of a(n) values */
static idx_t   a_size   = 0;      /* number of values stored (a_size = last_index+1) */
static idx_t   a_cap    = 0;      /* capacity of a_cache */

static seq_t *lucas_cache = NULL; /* cache of Lucas numbers */
static idx_t   lucas_size  = 0;    /* number of Lucas numbers computed so far */
static idx_t   lucas_cap   = 0;    /* capacity of lucas_cache */

/* index of the smallest m with Lucas(m) >= current k */
static idx_t   lucas_idx   = 0;

/* ------------------------------------------------------------------ */
/*  Helper: ensure lucas_cache has space for at least 'needed'+1 entries */
static void ensure_lucas_through(idx_t needed)
{
    if (needed < lucas_size) return;

    idx_t new_cap = lucas_cap ? lucas_cap * 2 : 16;
    while (new_cap <= needed) new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(lucas_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    lucas_cache = tmp;
    lucas_cap   = new_cap;

    /* compute missing Lucas numbers */
    while (lucas_size <= needed) {
        idx_t i = lucas_size;
        lucas_cache[i] = lucas_cache[i-1] + lucas_cache[i-2];
        ++lucas_size;
    }
}

/* ------------------------------------------------------------------ */
/*  Helper: ensure a_cache has space for at least 'needed'+1 entries  */
static void ensure_a_through(idx_t needed)
{
    if (needed < a_cap) return;

    idx_t new_cap = a_cap ? a_cap * 2 : 16;
    while (new_cap <= needed) new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;
    a_cap   = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Initialise caches lazily                                      */
static void init_caches(void)
{
    if (a_cache) return;          /* already initialised */

    /* a_cache */
    a_cache = (seq_t *)malloc(16 * sizeof(seq_t));
    if (!a_cache) abort();
    a_cap = 16;
    a_cache[0] = (seq_t)0;
    a_size = 1;                    /* a(0) stored */

    /* lucas_cache */
    lucas_cache = (seq_t *)malloc(16 * sizeof(seq_t));
    if (!lucas_cache) abort();
    lucas_cap = 16;
    lucas_cache[0] = (seq_t)2;
    lucas_cache[1] = (seq_t)1;
    lucas_size = 2;
    lucas_idx  = 0;                /* minimal m for k=0 or 1 is 0 */
}

/* ------------------------------------------------------------------ */
/*  Main function required by the build.                            */
seq_t a(idx_t n)
{
    init_caches();

    if (n < a_size) return a_cache[n];

    /* compute values incrementally up to n */
    for (idx_t k = a_size; k <= n; ++k) {
        /* move lucas_idx to the minimal m with Lucas(m) >= k */
        while (1) {
            if (lucas_idx >= lucas_size) {
                /* need more Lucas numbers */
                ensure_lucas_through(lucas_idx + 1);
            }
            if (lucas_cache[lucas_idx] < k) {
                ++lucas_idx;
            } else {
                break;              /* found minimal index */
            }
        }

        ensure_a_through(k);

        /* a(k) = a(k-1) + lucas_idx  (lucas_idx is the value A130242(k)) */
        a_cache[k] = a_cache[k-1] + (seq_t)lucas_idx;
    }

    a_size = n + 1;
    return a_cache[n];
}
