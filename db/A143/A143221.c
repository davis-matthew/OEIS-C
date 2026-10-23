/*
 * A143221  a(0)=1. For n >=1, a(n) = 0 if the binary representation of n
 *          occurs at least once in the concatenation of
 *          (a(0),a(1),...,a(n-1)). a(n) = 1 otherwise.
 * https://oeis.org/A143221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Transitive recurrence:
   a(0) = 1
   for n ≥ 1,
     a(n) = 0  if the binary representation of n occurs in
               the concatenation of a(0),a(1),…,a(n‑1);
     a(n) = 1  otherwise.
*/
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t      *cache   = NULL;   /* cached a(n) values */
static size_t      cache_cap = 0;    /* allocated cache size */
static size_t      cache_len = 0;    /* number of elements stored */

static char       *concat  = NULL;   /* concatenated bits of a(0) ... */
static size_t      concat_cap = 0;    /* allocated concat size */
static size_t      concat_len = 0;    /* current number of bits in concat */

static bool        initialized = false;

/* initialise storage for a(0) */
static void init(void)
{
    if (initialized)
        return;
    initialized = true;

    /* cache: at least one element for a(0) */
    cache_cap = 16;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    cache_len = 1;
    cache[0] = (seq_t)1;            /* a(0) = 1 */

    /* concatenated string: starts with "1" */
    concat_cap = 32;
    concat = (char*)malloc(concat_cap);
    concat_len = 1;
    concat[0] = '1';
    concat[1] = '\0';
}

/* ensure values up to n are computed and stored in cache */
static void ensure_up_to(idx_t n)
{
    init();                           /* guarantee a(0) exists */

    /* grow arrays as needed before starting the loop */
    if (cache_len <= n) {
        size_t new_cap = cache_cap;
        if (new_cap < n + 1)
            new_cap = n + 1;
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
    }
    if (concat_len + 65 > concat_cap) { /* enough space for any bit pattern + terminator */
        size_t new_cap = concat_cap * 2;
        if (new_cap < concat_len + 65)
            new_cap = concat_len + 65;
        char *tmp = (char*)realloc(concat, new_cap);
        concat = tmp;
        concat_cap = new_cap;
    }

    /* compute from the next missing index up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* binary representation of i in temporary buffer; bits are LSB first */
        char bits_buf[65];
        idx_t t = i;
        unsigned int bits = 0;
        while (t) {
            bits_buf[bits++] = (t & 1) ? '1' : '0';
            t >>= 1;
        }
        /* reverse to obtain MSB first */
        for (unsigned int l = 0; l < bits / 2; ++l) {
            char tmp_c = bits_buf[l];
            bits_buf[l] = bits_buf[bits - 1 - l];
            bits_buf[bits - 1 - l] = tmp_c;
        }
        bits_buf[bits] = '\0';               /* null‑terminate */

        /* check if pattern occurs in the current concatenation */
        bool found = strstr(concat, bits_buf) != NULL;
        seq_t val = found ? (seq_t)0 : (seq_t)1;

        cache[i] = val;                      /* store it */

        /* append this single bit to the concatenated string */
        if (concat_len + 1 >= concat_cap) {   /* need space for new bit and '\0' */
            size_t new_cap = concat_cap * 2;
            if (new_cap < concat_len + 2)
                new_cap = concat_len + 2;
            char *tmp = (char*)realloc(concat, new_cap);
            concat = tmp;
            concat_cap = new_cap;
        }
        concat[concat_len++] = (char)('0' + val);
        concat[concat_len] = '\0';
    }

    cache_len = n + 1;
}

/* required interface */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* a(0) is defined to be 1 */

    ensure_up_to(n);
    return cache[n];
}
