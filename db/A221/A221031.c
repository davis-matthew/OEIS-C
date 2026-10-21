/*
 * A221031  Majority value maps: number of n X 4 binary arrays indicating the
 *          locations of corresponding elements equal to at least half of
 *          their horizontal and antidiagonal neighbors in a random 0..2 n X 4
 *          array.
 * https://oeis.org/A221031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n-1) -139*a(n-2) +446*a(n-3) -825*a(n-4) +817*a(n-5)
 *            -168*a(n-6) -528*a(n-7) +480*a(n-8) +48*a(n-9) -208*a(n-10)
 *            +64*a(n-11) for n > 12, with initial values a(1)=7, a(2)=119,
 *            a(3)=1733, a(4)=24485, a(5)=345755, a(6)=4882030, a(7)=68933905,
 *            a(8)=973340015, a(9)=13743460075, a(10)=19405...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

/* Memoised cache */
static seq_t * _cache      = NULL;   /* array of size _cap, indices 0.._cap-1 */
static size_t _cap         = 0;      /* actual allocated capacity */
static idx_t  _computed_n  = 0;      /* highest index computed so far */

/* Ensure _cache can hold indices 0..n inclusive. */
static void _ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= _cap)
        return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* out of memory */
    _cache = tmp;
    _cap   = new_cap;
}

/* Initial 12 terms declared as constants */
static const seq_t _init[13] = {
    (seq_t)0ULL,                       /* unused index 0 */
    (seq_t)7ULL,                       /* a(1)   */
    (seq_t)119ULL,                     /* a(2)   */
    (seq_t)1733ULL,                    /* a(3)   */
    (seq_t)24485ULL,                   /* a(4)   */
    (seq_t)345755ULL,                  /* a(5)   */
    (seq_t)4882030ULL,                 /* a(6)   */
    (seq_t)68933905ULL,                /* a(7)   */
    (seq_t)973340015ULL,               /* a(8)   */
    (seq_t)13743460075ULL,             /* a(9)   */
    (seq_t)194056216948ULL,            /* a(10)  */
    (seq_t)2740053437074ULL,           /* a(11)  */
    (seq_t)38689267213364ULL           /* a(12)  */
};

/* Compute all terms up to and including n, memoising results. */
static void _compute_to(idx_t n)
{
    if (n <= _computed_n)
        return;

    _ensure_cache(n);

    /* Initialise base terms if not already done. */
    for (idx_t i = 1; i <= 12 && i <= n; ++i)
        if (i > _computed_n)
            _cache[i] = _init[i];

    /* Apply recurrence for indices > 12. */
    idx_t start = (_computed_n > 12) ? _computed_n + 1ULL : 13ULL;
    for (idx_t i = start; i <= n; ++i)
    {
        seq_t v = 0;
        /* Positive contributions */
        v += 22ULL * _cache[i-1];
        v += 446ULL * _cache[i-3];
        v += 817ULL * _cache[i-5];
        v += 480ULL * _cache[i-8];
        v += 48ULL  * _cache[i-9];
        v += 64ULL  * _cache[i-11];

        /* Negative contributions */
        v -= 139ULL * _cache[i-2];
        v -= 825ULL * _cache[i-4];
        v -= 168ULL * _cache[i-6];
        v -= 528ULL * _cache[i-7];
        v -= 208ULL * _cache[i-10];

        _cache[i] = v;
    }

    _computed_n = n;
}

/* Public access function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset is 1; value below offset is irrelevant */

    _compute_to(n);
    return _cache[n];
}
