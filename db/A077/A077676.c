/*
 * A077676  Squares using only squarefree digits (2, 3, 5, 6, 7).
 * https://oeis.org/A077676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th perfect square whose decimal digits belong to
 *            {2,3,5,6,7}. The sequence is infinite
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 152, u64 n <= 600,
 *            u128 n <= 600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* digits allowed in a square */
static const bool allowed_digit[10] = {
    /*0*/false,
    /*1*/false,
    /*2*/true,
    /*3*/true,
    /*4*/false,
    /*5*/true,
    /*6*/true,
    /*7*/true,
    /*8*/false,
    /*9*/false
};

/* check that all decimal digits of x belong to the allowed set */
static bool digits_ok(seq_t x)
{
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        if (!allowed_digit[d])
            return false;
        x /= 10;
    }
    return true;
}

/* append a square to the static cache */
static void cache_append(seq_t **cache, size_t *len, size_t *cap, seq_t val)
{
    if (*len + 1 > *cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (*cache) {
            for (size_t i = 0; i < *len; ++i) tmp[i] = (*cache)[i];
            free(*cache);
        }
        *cache = tmp;
        *cap = new_cap;
    }
    (*cache)[*len] = val;
    (*len)++;
}

/* a(n): n >= 1 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached list of squares */
    static size_t cache_len = 0;    /* number of cached squares */
    static size_t cache_cap = 0;    /* capacity of cache */
    static idx_t last_checked = 0;  /* last base number examined */

    /* continue searching until we have at least n squares cached */
    while (cache_len < (size_t)n) {
        last_checked++;                     /* next base to try */
        seq_t sq = (seq_t)last_checked * (seq_t)last_checked;
        if (digits_ok(sq)) {
            cache_append(&cache, &cache_len, &cache_cap, sq);
        }
    }
    return cache[n - 1];
}
