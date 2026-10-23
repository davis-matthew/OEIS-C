/*
 * A010342  Numbers k such that all terms in the periodic part of the
 *          continued fraction for sqrt(k) except the final term are 1.
 * https://oeis.org/A010342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by merging the infinite increasing * streams *
 *            k(d,m) = ((d*F(m+1)+1)/2)^2 + d*F(m) + 1 * for each admissible m
 *            ≡ 0 or 1 (mod 3) and for all odd d≥1. * The streams are merged
 *            in increasing order to produce the * sequence A010342. See the
 *            OEIS entry for details
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*  The types seq_t and idx_t are supplied by the build system and
 *  should not be re‑defined here.  Only use them as declared types.  */

/*  Constants  */
#define MAX_M 80  /* sufficiently large to generate >10 000 terms
                    (increase if you need more).  */

/*  Structure representing one stream for a fixed m  */
typedef struct {
    seq_t Fm;     /* F(m)                           */
    seq_t Fmp1;   /* F(m+1)                         */
    seq_t d;      /* current odd d (≥1)             */
    seq_t next_k; /* next k value from this stream  */
} stream_t;

/*  Static state  */
static bool            initialized = false;
static stream_t        streams[MAX_M+1];
static size_t         stream_cnt = 0;

static seq_t *cache   = NULL;   /* array of computed terms   */
static size_t  cache_len  = 0;   /* number computed           */
static size_t  cache_cap  = 0;   /* current capacity          */

/*  Compute the next k value for a stream  */
static void compute_next_k(stream_t *s)
{
    /*  q = (d*F(m+1)+1)/2  (numerator is even)  */
    seq_t numer = s->d * s->Fmp1 + 1;
    seq_t q     = numer >> 1;
    s->next_k = q * q + s->d * s->Fm + 1;
}

/*  Initialise global data: Fibonacci numbers, streams, cache  */
static void init(void)
{
    /*  Fibonacci numbers up to MAX_M+1  */
    seq_t fib[MAX_M + 2];
    fib[0] = 0;
    fib[1] = 1;
    for (int i = 2; i <= MAX_M + 1; ++i)
        fib[i] = fib[i-1] + fib[i-2];

    /*  Build a stream for each admissible m  */
    size_t idx = 0;
    for (int m = 1; m <= MAX_M; ++m) {
        if (m % 3 == 2) continue;  /* skip m ≡ 2 (mod 3) */
        streams[idx].Fm   = fib[m];
        streams[idx].Fmp1 = fib[m+1];
        streams[idx].d    = 1;              /* first odd d */
        compute_next_k(&streams[idx]);
        ++idx;
    }
    stream_cnt = idx;

    /*  Initialise cache  */
    cache_cap = 64;
    cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    cache_len = 0;

    initialized = true;
}

/*  Return the smallest next_k among all streams  */
static seq_t next_term(void)
{
    size_t best = 0;
    for (size_t i = 1; i < stream_cnt; ++i) {
        if (streams[i].next_k < streams[best].next_k)
            best = i;
    }
    seq_t val = streams[best].next_k;

    /*  Advance that stream  */
    streams[best].d += 2;           /* next odd d */
    compute_next_k(&streams[best]);

    return val;
}

/*  Generate cached values up to at least the n‑th term  */
static void generate_cache_up_to(idx_t n)
{
    while (cache_len < n) {
        seq_t v = next_term();

        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = v;
    }
}

/*  Public function required by the exercise  */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 0) return 0;  /* out of range, defined as 0 */

    generate_cache_up_to(n);
    return cache[n-1];
}
