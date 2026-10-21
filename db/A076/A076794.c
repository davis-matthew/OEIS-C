/*
 * A076794  Numbers of the form Sum_{k=1..m} prime(r)^prime(k) for some values
 *          of m and r.
 * https://oeis.org/A076794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate all sums
   S(r) = { sum_{k=1..m} prime(r)^prime(k) | m >= 1 } ,
   merge the sequences in increasing order, remove duplicates,
   and return the n‑th element (1‑based).  */

/*=========================================================================*/
/*  Header inclusions                                                    */
/*=========================================================================*/
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/big_pow.h"           /* seq_t big_pow(seq_t, seq_t) */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t, uint64_t**, size_t*) */

/*=========================================================================*/
/*  Heap element structure                                               */
/*=========================================================================*/
typedef struct {
    seq_t sum;   /* current cumulative sum of the sequence */
    idx_t r;     /* base prime index (1‑based) */
    idx_t m;     /* number of terms already summed (m >= 1) */
    seq_t base;  /* the base prime prime(r) */
} HeapItem;

/*=========================================================================*/
/*  Static state                                                          */
/*=========================================================================*/
static seq_t *cache       = NULL;   /* cached sequence values      */
static idx_t  cached_len  = 0;      /* number of values cached     */
static size_t cache_cap   = 0;      /* capacity of cache array     */

static uint64_t *base_primes = NULL;/* array of primes for base and exponents */
static size_t   base_count   = 0;    /* number of primes obtained */

static HeapItem *heap = NULL;       /* min‑heap of active sequences */
static size_t    heap_size = 0;     /* current heap size            */
static size_t    heap_cap   = 0;     /* capacity of heap              */

/*=========================================================================*/
/*  Heap operations                                                       */
/*=========================================================================*/
static inline void sift_up(size_t i)
{
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].sum <= heap[i].sum) break;
        HeapItem t = heap[p]; heap[p] = heap[i]; heap[i] = t;
        i = p;
    }
}

static inline void sift_down(size_t i)
{
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= heap_size) break;
        size_t r = l + 1;
        size_t s = (r < heap_size && heap[r].sum < heap[l].sum) ? r : l;
        if (heap[i].sum <= heap[s].sum) break;
        HeapItem t = heap[i]; heap[i] = heap[s]; heap[s] = t;
        i = s;
    }
}

static inline void heap_push(HeapItem x)
{
    if (heap_size == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 8;
        heap = (HeapItem *)realloc(heap, heap_cap * sizeof(HeapItem));
    }
    heap[heap_size++] = x;
    sift_up(heap_size - 1);
}

static inline HeapItem heap_pop(void)
{
    HeapItem top = heap[0];
    heap[0] = heap[--heap_size];
    sift_down(0);
    return top;
}

/*=========================================================================*/
/*  Sequence generator                                                    */
/*=========================================================================*/
static void generate_upto(idx_t n)
{
    if (n <= cached_len) return;

    /* Lazily build the prime array and heap on first use */
    if (!base_primes) {
        size_t cnt;
        uint64_t *pr = NULL;
        /* Choose a limit large enough to provide many base primes.
           300 000 gives about 26 000 primes – plenty for n<=10000. */
        build_primes_upto(300000, &pr, &cnt);
        base_primes = pr;
        base_count  = cnt;

        /* initialise heap with the first term of every base sequence */
        heap_cap  = base_count;
        heap      = (HeapItem *)malloc(heap_cap * sizeof(HeapItem));
        heap_size = 0;
        for (idx_t r = 1; r <= base_count; ++r) {
            seq_t base   = (seq_t)base_primes[r - 1];
            seq_t first  = big_pow(base, (seq_t)base_primes[0]); /* base^2 */
            HeapItem h = {first, r, 1, base};
            heap_push(h);
        }
    }

    /* Merge until we have enough unique terms */
    while (cached_len < n) {
        HeapItem cur = heap_pop();
        seq_t val    = cur.sum;

        /* deduplicate equal values */
        if (!cached_len || val != cache[cached_len - 1]) {
            if (cached_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 32;
                cache     = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cached_len++] = val;
        }

        /* generate next element of the same base sequence, if possible */
        idx_t next_m = cur.m + 1;
        if (next_m <= base_count) {   /* we have enough primes for exponent */
            seq_t base    = cur.base;
            seq_t exp     = (seq_t)base_primes[next_m - 1];
            seq_t next_pow = big_pow(base, exp);
            seq_t next_sum = cur.sum + next_pow;
            HeapItem h = {next_sum, cur.r, next_m, base};
            heap_push(h);
        }
    }
}

/*=========================================================================*/
/*  Public function                                                        */
/*=========================================================================*/
seq_t a(idx_t n)
{
    generate_upto(n);
    return cache[n - 1];
}
