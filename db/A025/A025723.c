/*
 * A025723  Index of 7^n within the sequence of the numbers of the form
 *          5^i*7^j.
 * https://oeis.org/A025723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=1..n} ceil(k*log_5(7))
 * Range:     u8 n <= 19, u16 n <= 328, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- Big integer for multiplication by small constants ---------- */
typedef struct {
    uint32_t *d;   /* little‑endian digits, d[0] least significant */
    size_t len;    /* number of used digits */
} BigInt;

/* initialise BigInt to 1 */
static void big_init_one(BigInt *x)
{
    x->len = 1;
    x->d   = (uint32_t *)malloc(sizeof(uint32_t));
    x->d[0] = 1;
}

/* multiply a big integer by a small constant (<= 10) */
static void big_mul_small(BigInt *x, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)prod;
        carry   = prod >> 32;
    }
    if (carry) {
        x->d = (uint32_t *)realloc(x->d, sizeof(uint32_t) * (x->len + 1));
        x->d[x->len] = (uint32_t)carry;
        x->len++;
    }
}

/* compare two big integers: return -1 if a<b, 0 if equal, 1 if a>b */
static int big_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len)
        return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i])
            return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* ---------- Sequence implementation ---------- */
static seq_t *cache = NULL;      /* cached a(n) values */
static size_t cache_cap = 0;     /* allocated entries */
static idx_t cache_max_n = 0;    /* highest n for which cache[ n ] is valid */

static BigInt big5;              /* current 5^m */
static BigInt big7;              /* current 7^k (k is current index) */
static idx_t m_cur = 0;          /* current m such that 5^m > 7^k */

/* ensure cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise static data on first call */
static void init_static(void)
{
    static int initialized = 0;
    if (initialized)
        return;
    cache = NULL;
    cache_cap = 0;
    cache_max_n = 0;
    big_init_one(&big5);
    big_init_one(&big7);
    m_cur = 0;
    ensure_cache(0);
    cache[0] = 1;           /* a(0) = 1 */
    cache_max_n = 0;
    initialized = 1;
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_static();

    if (n <= cache_max_n)
        return cache[n];

    /* compute values up to n */
    ensure_cache(n);
    for (idx_t k = cache_max_n + 1; k <= n; ++k) {
        /* advance 7^k */
        big_mul_small(&big7, 7);

        /* increase m while 5^m <= 7^k */
        while (big_cmp(&big5, &big7) <= 0) {
            ++m_cur;
            big_mul_small(&big5, 5);
        }

        /* a(k) = a(k-1) + ceil(k*alpha) = a(k‑1) + m_cur */
        cache[k] = cache[k - 1] + (seq_t)m_cur;
    }
    cache_max_n = n;
    return cache[n];
}
