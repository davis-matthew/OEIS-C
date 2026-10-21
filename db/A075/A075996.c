/*
 * A075996  Primes of the form 2 + 2^n + 3^n.
 * https://oeis.org/A075996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the form 2 + 2^k + 3^k (k starting at 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

static inline void append_prime(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len >= *cap) {
        size_t newcap = (*cap == 0) ? 128 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!tmp) abort();            /* Out of memory – give up. */
        *arr = tmp;
        *cap = newcap;
    }
    (*arr)[(*len)++] = val;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Static storage for cache and state */
    static seq_t *prime_cache = NULL;
    static size_t prime_len  = 0;
    static size_t prime_cap  = 0;
    static seq_t  cur_p2 = 0;   /* 2^k */
    static seq_t  cur_p3 = 0;   /* 3^k */
    static idx_t  k       = 0;   /* current exponent (1‑based) */
    static int    init    = 0;

    /* One‑time initialization */
    if (!init) {
        prime_cap = 128;
        prime_cache = (seq_t *)calloc(prime_cap, sizeof(seq_t));
        if (!prime_cache) abort();
        cur_p2 = 2;          /* 2^1 */
        cur_p3 = 3;          /* 3^1 */
        k      = 1;
        init = 1;
    }

    /* Generate more primes until we have at least n of them */
    while (prime_len < (size_t)n) {
        /* val = 2 + 2^k + 3^k */
        seq_t val = cur_p2 + cur_p3 + (seq_t)2;

        if (is_prime_big(val))
            append_prime(&prime_cache, &prime_len, &prime_cap, val);

        /* Prepare next exponent, checking for overflow on fixed‑width types */
        /* multiplications */
        seq_t new_p2 = cur_p2 + cur_p2; /* cur_p2 * 2 */
        seq_t new_p3 = cur_p3 + cur_p3; /* cur_p3 * 2 */
        if (new_p3 < cur_p3) new_p3 = cur_p3;           /* detect wrap */
        new_p3 = new_p3 + cur_p3;                       /* now multiply by 3 */
        if (new_p3 < cur_p3 || new_p3 < new_p3) break;  /* overflow */

        if (new_p2 < cur_p2) break; /* overflow for power of two */

        cur_p2 = new_p2;
        cur_p3 = new_p3;
        ++k;
    }

    if (prime_len < (size_t)n) return 0;  /* cannot compute more terms */

    return prime_cache[(size_t)n - 1];
}
