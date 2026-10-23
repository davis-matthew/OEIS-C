/*
 * A105831  For n>2, a(n) not appeared previously is such that
 *          a(n-1)^2+4*a(n-2)*a(n) is a minimal square, a(1)=1, a(2)=6.
 * https://oeis.org/A105831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A105831
 *  a(1) = 1, a(2) = 6.
 *  For n>2, a(n) is the smallest positive integer not appearing previously
 *  such that a(n-1)^2 + 4*a(n-2)*a(n) is a perfect square.
 *  The implementation evaluates the recurrence using a simple scan over
 *  candidates derived from the difference of two consecutive squares.
 *  Memoisation of already-computed terms allows arbitrary call order.
 */

#include <stdlib.h>
#include <stdbool.h>

/* dynamic array holding all already-computed terms (index 1..cache_len) */
static seq_t *cache = NULL;   /* array of seq_t */
static idx_t  cache_len = 0;  /* last filled index (>=2 after init) */
static size_t cache_cap = 0;  /* allocated capacity */

/* ensure that 'cache' has capacity for at least 'need' elements (1‑based) */
static void cache_ensure(idx_t need)
{
    if ((size_t)need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while ((size_t)need > new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise the base of the sequence and the cache */
static void init_sequence(void)
{
    if (cache_len != 0) return;        /* already initialised */
    cache_ensure(3);                   /* indices 0..2 used */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)6;
    cache_len = 2;
}

/* test whether a value is already in the cache */
static bool used_present(seq_t val)
{
    for (idx_t i = 1; i <= cache_len; ++i) {
        if (cache[i] == val) return true;
    }
    return false;
}

/* compute all terms up to index n (if needed) */
static void compute_up_to(idx_t n)
{
    init_sequence();
    while (cache_len < n) {
        seq_t d = cache[cache_len];     /* a(n-1) */
        seq_t c = cache[cache_len-1];   /* a(n-2) */
        seq_t denom = (seq_t)4 * c;     /* 4 * a(n-2) */
        idx_t k = 1;
        for (;;) {
            seq_t ks = (seq_t)k;
            seq_t two_d = (seq_t)2 * d;
            seq_t num = ks * (two_d + ks);         /* k*(2*d + k) */
            if ((num % denom) == 0) {              /* integer a?   */
                seq_t a = num / denom;
                if (a > 0 && !used_present(a)) {
                    cache_ensure(cache_len + 2);
                    cache[cache_len + 1] = a;
                    ++cache_len;
                    break;
                }
            }
            ++k;                                   /* next candidate */
        }
    }
}

/* required external function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safe sentinel, offset is 1 */
    compute_up_to(n);
    return cache[n];
}
