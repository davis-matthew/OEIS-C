/*
 * A099972  Write 1/sqrt(2) as a binary fraction; read this from left to right
 *          and whenever a 1 appears, note the integer formed by reading
 *          leftwards from that 1.
 * https://oeis.org/A099972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = integer formed by the binary bits of 1/√2 up to the *
 *            n‑th 1, read in reverse order
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 13, u64 n <= 32, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* current cache capacity */

/* 1/√2 bits – state of the binary square‑root algorithm for √2 */
static seq_t P = 0;              /* current root (integer + fractional bits) */
static seq_t R = 0;              /* current remainder                     */

/* reverse‑prefix value and next bit weight */
static seq_t rev   = 0;          /* reversed bits seen so far            */
static seq_t pow2  = 0;          /* 2^(bit_position-1), updated each loop */

/* ensure the cache can hold `need` entries */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache      = tmp;
    cache_cap  = new_cap;
}

/* initialise state once */
static void init(void)
{
    ensure_cache(16);
    cache[0]   = 1;          /* a(0) = 1 */
    cache_len  = 1;

    /* start with the first bit of 1/√2 */
    rev  = 1;                /* first reversed prefix (‘1’)          */
    pow2 = 1;                /* weight for the next bit (2^1)        */

    /* binary sqrt of 2: initial root and remainder for √2 */
    P = 1;                   /* floor(√2) = 1                       */
    R = 1;                   /* 2 – 1² = 1                          */
}

/* main function required by the framework */
seq_t a(idx_t n)
{
    static bool first = true;
    if (first) { init(); first = false; }

    while (cache_len <= (size_t)n)
    {
        /* next bit of √2 (fractional part) */
        R <<= 2;                                                         /* bring down two zero bits */
        seq_t cand = (P << 2) | 1;                                       /* (P << 1) and test 1   */
        bool bit;
        if (R >= cand) {
            R -= cand;
            P = (P << 1) | 1;                                            /* bit = 1 */
            bit = true;
        } else {
            P <<= 1;                                                     /* bit = 0 */
            bit = false;
        }

        /* update reversed‑prefix state – shift weight and add if bit=1 */
        pow2 <<= 1;                                                       /* 2^(position-1)       */
        if (bit) {
            rev += pow2;                                                 /* include new 1 bit    */
            ensure_cache(cache_len + 1);
            cache[cache_len++] = rev;                                    /* store new term      */
        }
    }

    return cache[(size_t)n];
}
