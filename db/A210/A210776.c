/*
 * A210776  Number of partitions of 2^n into powers of 2 less than or equal to
 *          128.
 * https://oeis.org/A210776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the number of partitions of 2^n into powers of 2
 * not exceeding 128.
 *
 * The ordinary generating function is
 *     A(x) = P(x) / Product_{j=0..7} (1 - 2^j x)
 * with P(x) a degree‑13 polynomial.
 * For n ≥ 14 the sequence satisfies the linear recurrence
 *
 *     a(n) =  Σ_{k=1}^{8} (-1)^{k+1} S_k · a(n-k)
 *
 * where S_k is the k‑th elementary symmetric sum of the set
 * {1,2,4,8,16,32,64,128}.  The first 14 terms are obtained by a
 * simple partition DP for 2^n ≤ 8192.
 *
 * The function keeps a static cache so that repeated calls in any
 * order are O(n) after the first initialization.
 */

#include <stdlib.h>
#include <stdint.h>

static const uint64_t POWERS[8] = {1ULL,2ULL,4ULL,8ULL,16ULL,32ULL,64ULL,128ULL};

static uint64_t SYM[9];   /* symmetric sums S_k (k=0..8), S_0 = 1 */
static int      sym_init = 0;

static seq_t *cache = NULL;       /* cache of a(n) values */
static size_t cache_size = 0;     /* number of elements stored */
static size_t cache_cap  = 0;     /* capacity of cache array */

/* ------------------------------------------------------------ */
/* Compute symmetric sums S_k of {1,2,4,8,16,32,64,128}          */
/* ------------------------------------------------------------ */
static void init_sym()
{
    if (sym_init) return;
    uint64_t e[9] = {0};
    e[0] = 1;
    for (int i = 0; i < 8; ++i) {
        uint64_t v = POWERS[i];
        int j = 8;
        for (; j >= 1; --j) {
            e[j] = e[j] + e[j-1] * v;
        }
    }
    for (int k = 0; k <= 8; ++k) SYM[k] = e[k];
    sym_init = 1;
}

/* ------------------------------------------------------------ */
/* Initialise cache with a(0)..a(13) by a simple DP              */
/* ------------------------------------------------------------ */
static void init_cache()
{
    /* Ensure symmetric sums are ready */
    init_sym();

    /* Allocate cache for first 14 terms (indices 0..13) */
    cache_cap  = 16;
    cache_size = 0;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    if (!cache) return;   /* OOM – unlikely on contest platform */

    /* DP for partitions of numbers up to 8192 (2^13) */
    const size_t MAXVAL = 8192;        /* 2^13 */
    seq_t *part = (seq_t *)calloc(MAXVAL + 1, sizeof(seq_t));
    if (!part) return;
    part[0] = 1;                     /* one way to make 0 */
    for (size_t i = 0; i < 8; ++i) {
        uint64_t coin = POWERS[i];
        for (size_t v = coin; v <= MAXVAL; ++v) {
            part[v] = part[v] + part[v - coin];
        }
    }
    /* Store a(0)..a(13) in cache */
    for (int n = 0; n <= 13; ++n) {
        cache[cache_size++] = part[1u << n];
    }
    free(part);
}

/* ------------------------------------------------------------ */
/* Grow cache up to index n (inclusive)                         */
/* ------------------------------------------------------------ */
static void ensure_cache(idx_t n)
{
    if (!cache && !cache_cap) init_cache();  /* first call */

    while (cache_size <= (size_t)n) {
        /* Grow array if necessary */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_ptr) return;   /* OOM – ignore */
            cache = new_ptr;
            cache_cap = new_cap;
        }

        /* Compute next term */
        seq_t val;
        if (cache_size <= 13) {
            /* already in cache; should not happen */
            val = cache[cache_size];
        } else {
            /* recurrence a(n) = Σ_{k=1}^{8} (-1)^{k+1} S_k * a(n-k) */
            seq_t sumPos = (seq_t)0;
            seq_t sumNeg = (seq_t)0;
            for (int k = 1; k <= 8; ++k) {
                seq_t term = cache[cache_size - k] * (seq_t)SYM[k];
                if (k & 1)          /* k odd -> + */
                    sumPos = sumPos + term;
                else                /* k even -> - */
                    sumNeg = sumNeg + term;
            }
            val = sumPos - sumNeg;  /* subtraction should be safe; final result >0 */
        }
        cache[cache_size++] = val;
    }
}

/* ------------------------------------------------------------ */
/* Public function                                                  */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
