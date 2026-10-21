/*
 * A083162  a(n) is the smallest unused proper divisor or proper multiple of n
 *          such that a(n)/n != a(m)/m for all m < n.
 * https://oeis.org/A083162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A083162: a(n) is the smallest unused proper divisor or proper multiple of n
 * such that the ratio a(n)/n is distinct from all earlier ratios.
 * The implementation is a simple greedy search that keeps the set of
 * previously used values and ratios in dynamic arrays.
 */

#include <stdlib.h>
#include <stddef.h>

/* ======== helpers ======== */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

struct Fraction {
    seq_t num;
    seq_t den;
};

/* ======== global dynamic arrays ======== */
static seq_t *value_cache = NULL;     /* index 1 .. have computed numbers */
static size_t cache_len = 0;          /* number of elements stored (index 0 unused) */
static size_t cache_cap = 0;

static struct Fraction *frac_cache = NULL;  /* all previously used reduced ratios */
static size_t frac_len = 0;
static size_t frac_cap = 0;

/* ================== dynamic array growth ================== */
static void ensure_cache_cap(size_t min_cap)
{
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < min_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(value_cache, new_cap * sizeof(seq_t));
        value_cache = tmp;
        cache_cap = new_cap;
    }
}

static void ensure_frac_cap(size_t min_cap)
{
    if (frac_cap < min_cap) {
        size_t new_cap = frac_cap ? frac_cap * 2 : 16;
        while (new_cap < min_cap) new_cap *= 2;
        struct Fraction *tmp = (struct Fraction *)realloc(frac_cache, new_cap * sizeof(struct Fraction));
        frac_cache = tmp;
        frac_cap = new_cap;
    }
}

/* ================== queries on used sets ================== */
static int used_value(seq_t v)
{
    for (size_t i = 1; i < cache_len; ++i)
        if (value_cache[i] == v) return 1;
    return 0;
}

static int used_fraction(seq_t num, seq_t den)
{
    for (size_t i = 0; i < frac_len; ++i) {
        if (frac_cache[i].num == num && frac_cache[i].den == den) return 1;
    }
    return 0;
}

/* ================== compute a(n) for next index ======== */
static seq_t compute_next(idx_t n)
{
    /* 1. Try proper divisors in increasing order */
    for (idx_t d = 1; d < n; ++d) {
        if (n % d != 0) continue;          /* not a divisor */
        seq_t cand = (seq_t)d;
        if (used_value(cand)) continue;      /* already used */

        seq_t g = gcd_seq_t(cand, (seq_t)n);
        seq_t num = cand / g;
        seq_t den = (seq_t)n / g;
        if (used_fraction(num, den)) continue;   /* ratio already used */

        return cand;   /* found the smallest divisor meeting the rules */
    }

    /* 2. If no divisor works, try proper multiples n*k (k >= 2) */
    for (seq_t k = 2;; ++k) {
        seq_t cand = (seq_t)n * k;
        if (used_value(cand)) continue;          /* value already used */

        seq_t num = k;      /* ratio is integer k/1 */
        seq_t den = 1;
        if (used_fraction(num, den)) continue;   /* ratio already used */

        return cand;   /* found a multiple satisfying the conditions */
    }
}

/* ===================== public function ===================== */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* n >= 1 as per specification */

    /* initialise caches on first call */
    if (cache_len == 0) {
        cache_len = 1;   /* placeholder for index 1 */
        ensure_cache_cap(32);
        value_cache[0] = 0;   /* unused */
        ensure_frac_cap(32);
    }

    /* extend caches if needed up to index n */
    if ((size_t)n + 1 > cache_cap) {
        ensure_cache_cap((size_t)n + 1);
    }

    for (idx_t cur = cache_len; cur <= n; ++cur) {
        seq_t val = compute_next(cur);

        /* store the newly computed value */
        ensure_cache_cap((size_t)cur + 1);
        value_cache[cur] = val;
        cache_len = cur + 1;

        /* compute its reduced ratio and store it */
        seq_t g = gcd_seq_t(val, (seq_t)cur);
        seq_t num = val / g;
        seq_t den = (seq_t)cur / g;

        ensure_frac_cap(frac_len + 1);
        frac_cache[frac_len].num = num;
        frac_cache[frac_len].den = den;
        ++frac_len;
    }

    return value_cache[n];
}
