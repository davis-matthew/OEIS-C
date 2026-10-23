/*
 * A041739  Denominators of continued fraction convergents to sqrt(389).
 * https://oeis.org/A041739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n‑th convergent of sqrt(389)
 *            Recurrence: a(0)=1, a(1)=a1 (the first continued‑fraction
 *            coefficient); for n≥2, a(n)=a_n·a(n‑1) + a(n‑2). The
 *            coefficients a_k are obtained from the simple continued fraction
 *            expansion of √389, whose periodic part ends when the coefficient
 *            e...
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 26, u64 n <= 53, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Storage for the periodic continued‑fraction coefficients a1,a2,… */
static bool     _cf_init  = false;
static uint32_t *_cf_terms = NULL;      /* index 0 → a1 */
static size_t   _cf_len   = 0;
static size_t   _cf_cap   = 0;

/* Storage for already computed denominators q_n */
static seq_t   *_cache     = NULL;      /* q_0 at [_cache[0]] */
static size_t   _cache_len = 0;         /* number of terms stored */
static size_t   _cache_cap = 0;

/* --------------------------------------------------------------------- */
/* initialise the list of periodic continued‑fraction coefficients for */
/* sqrt(389).  The list ends with the term 38.                             */

static void _init_contfrac(void)
{
    if (_cf_init) return;
    _cf_init = true;

    const uint64_t N = 389;
    uint64_t a0 = 0;                    /* floor(sqrt(389)) */
    while ((a0 + 1) * (a0 + 1) <= N) ++a0;

    uint64_t m = 0, d = 1;
    uint64_t a = a0;                     /* current coefficient */

    /* generate until we hit 2*a0 */
    for (;;)
    {
        /* next coefficients */
        m = d * a - m;
        d = (N - m * m) / d;
        a = (a0 + m) / d;

        /* store a */
        if (_cf_len == _cf_cap) {
            _cf_cap = _cf_cap ? _cf_cap * 2 : 4;
            uint32_t *tmp = (uint32_t *)realloc(_cf_terms, _cf_cap * sizeof(uint32_t));
            _cf_terms = tmp;
        }
        _cf_terms[_cf_len++] = (uint32_t)a;

        if (a == 2 * a0) break;   /* end of period */
    }
}

/* --------------------------------------------------------------------- */
/* initialise the cache for the denominators.  Only q_0 = 1 is needed.   */

static void _init_cache(void)
{
    if (_cache_len) return;        /* already initialised */

    _cache_cap = 4;
    _cache = (seq_t *)malloc(_cache_cap * sizeof(seq_t));
    _cache[0] = (seq_t)1;          /* q_0 = 1 */
    _cache_len = 1;
}

/* --------------------------------------------------------------------- */
/* ensure that denominators up to index n (inclusive) are computed.      */

static void _ensure_cache(idx_t n)
{
    if (_cache_len > n) return;     /* already present */

    /* compute missing terms */
    while (_cache_len <= n)
    {
        /* reallocate if needed */
        if (_cache_len == _cache_cap) {
            _cache_cap = _cache_cap ? _cache_cap * 2 : 4;
            _cache = (seq_t *)realloc(_cache, _cache_cap * sizeof(seq_t));
        }

        /* previous two denominators */
        seq_t q_prev     = _cache[_cache_len - 1];
        seq_t q_prevprev = (_cache_len >= 2) ? _cache[_cache_len - 2] : (seq_t)0;

        /* coefficient a_k where k = current index (_cache_len) */
        uint32_t term = _cf_terms[(_cache_len - 1) % _cf_len];

        /* recursion: q_k = a_k * q_{k-1} + q_{k-2} */
        seq_t q_new = (seq_t)term * q_prev + q_prevprev;
        _cache[_cache_len++] = q_new;
    }
}

/* --------------------------------------------------------------------- */
/* public function required by the OEIS framework.                        */

seq_t a(idx_t n)
{
    _init_contfrac();   /* create the coefficient list once */
    _init_cache();      /* initialise cache with q_0 = 1 */
    _ensure_cache(n);   /* compute up to requested index */
    return _cache[n];
}
