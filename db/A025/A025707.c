/*
 * A025707  Index of 5^n within the sequence of the numbers of the form
 *          5^i*6^j.
 * https://oeis.org/A025707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=0..n} ceil(k * log_6(5))
 * Range:     u8 n <= 22, u16 n <= 380, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence by incrementally comparing 5^k and 6^m.
   Big integers are used only for the comparison; the sequence values
   themselves are kept in the user‑supplied type seq_t. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define BIG_BASE 1000000000U   /* 1e9 < 2^32 */

typedef struct {
    uint32_t *d;   /* little‑endian digits, each < BIG_BASE */
    size_t len;    /* number of used digits */
} BigInt;

/* initialise BigInt to the integer value 'val' (non‑zero) */
static void big_init(BigInt *b, uint32_t val)
{
    b->len = 1;
    b->d   = (uint32_t *)malloc(sizeof(uint32_t));
    b->d[0] = val;
}

/* free memory allocated for BigInt */
static void big_free(BigInt *b)
{
    free(b->d);
    b->d = NULL; b->len = 0;
}

/* multiply BigInt by small factor (<= 10), modifying it in place */
static void big_mul_small(BigInt *b, uint32_t factor)
{
    uint64_t carry = 0, prod;
    for (size_t i = 0; i < b->len; ++i)
    {
        prod   = (uint64_t)b->d[i] * factor + carry;
        b->d[i] = (uint32_t)(prod % BIG_BASE);
        carry   = prod / BIG_BASE;
    }
    while (carry)
    {
        b->d = (uint32_t *)realloc(b->d, (b->len + 1) * sizeof(uint32_t));
        b->d[b->len] = (uint32_t)(carry % BIG_BASE);
        carry       /= BIG_BASE;
        ++b->len;
    }
}

/* compare two BigInts: return -1 if a<b, 0 if a==b, 1 if a>b */
static int big_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len)
        return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0; )
    {
        if (a->d[i] != b->d[i])
            return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* The memoised sequence accessor. */
seq_t a(idx_t n)
{
    /* static state --------------------------------------- */
    static seq_t *cache = NULL;      /* cached a(k) values */
    static idx_t  cache_len = 0;     /* number of cached entries */

    static BigInt pow5, pow6;        /* 5^k and 6^m */
    static idx_t  m_cur = 0;         /* current ceil(k*log6(5)) for k=cache_len-1 */

    static bool initialized = false;

    /* initialise first time --------------------------------*/
    if (!initialized) {
        /* cache[0] = 1 */
        cache      = (seq_t *)malloc(sizeof(seq_t));
        cache[0]   = (seq_t)1;
        cache_len  = 1;

        big_init(&pow5, 1);
        big_init(&pow6, 1);
        m_cur = 0;
        initialized = true;
    }

    /* extend cache if necessary --------------------------------*/
    if (n + 1 > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure path */
        cache = tmp;

        /* compute missing terms incrementally */
        for (idx_t k = cache_len; k <= n; ++k)
        {
            /* update 5^k */
            big_mul_small(&pow5, 5U);

            /* increase m_cur while 5^k > 6^m_cur */
            while (big_cmp(&pow5, &pow6) > 0) {
                ++m_cur;
                big_mul_small(&pow6, 6U);
            }

            /* a(k) = a(k-1) + m_cur */
            cache[k] = cache[k - 1] + (seq_t)m_cur;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
