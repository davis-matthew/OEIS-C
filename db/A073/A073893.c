/*
 * A073893  a(10k) = 0 and if n is not a multiple of 10 then a(n) is the
 *          smallest positive integer such that the concatenation of the first
 *          n terms is a multiple of n.
 * https://oeis.org/A073893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) is 0 when n is a multiple of 10; otherwise it is the *
 *            smallest positive integer m such that the decimal concatenation
 *            of * a(1),a(2),…,a(n) is a multiple of n. The algorithm computes
 *            the * concatenation modulo n, then searches for the smallest m.
 *            It uses * only integer arithmetic and caches...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* for malloc, realloc, free */
#include <stdint.h>          /* for uint64_t */
#include <stdbool.h>         /* for bool */

#include "../utils/digit_len.h"   /* int digit_len(seq_t x) */

seq_t a(idx_t n) {
    /* Static cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_cnt = 0;

    /* Grow cache if necessary */
    if (n > cached_cnt) {
        idx_t new_cnt = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cnt * sizeof(seq_t));
        if (!new_cache) exit(1); /* out‑of‑memory */
        cache = new_cache;
        for (idx_t i = cached_cnt; i < new_cnt; ++i)
            cache[i] = 0; /* initialise new entries */
        cached_cnt = new_cnt;
    }

    /* Return cached value if available */
    if (cache[n - 1] != 0 || (n % 10 == 0 && cache[n - 1] == 0))
        return cache[n - 1];

    /* Base case: n == 1 is handled by the general algorithm below */
    if (n == 1) {
        cache[0] = 1;
        return 1;
    }

    /* ---------- Compute C_{n-1} modulo n ---------- */
    seq_t mod_val = (seq_t)n;          /* n as seq_t for arithmetic */
    seq_t cur_mod = 0;                 /* C_{k} mod n for current k */

    /* Pre‑compute powers of 10 modulo n up to the maximum digit length
     * we expect (here we cap at 20 which is ample for n <= 10000) */
    const int MAX_DIG = 20;
    seq_t pow10_mod[MAX_DIG + 1];
    pow10_mod[0] = 1 % mod_val;
    for (int d = 1; d <= MAX_DIG; ++d)
        pow10_mod[d] = (pow10_mod[d - 1] * 10) % mod_val;

    for (idx_t i = 1; i < n; ++i) {
        seq_t term = cache[i - 1];
        int d = digit_len(term);
        if (d > MAX_DIG) {
            /* Extend the power array if needed */
            for (int dd = MAX_DIG + 1; dd <= d; ++dd)
                pow10_mod[dd] = (pow10_mod[dd - 1] * 10) % mod_val;
        }
        cur_mod = (cur_mod * pow10_mod[d] + (term % mod_val)) % mod_val;
    }

    /* If n is a multiple of 10 we know the answer is 0 */
    if (n % 10 == 0) {
        cache[n - 1] = 0;
        return 0;
    }

    /* ---------- Search for the smallest m ---------- */
    seq_t pow10_val_prev = 1;                  /* 10^(d-1) */
    seq_t pow10_val_curr = 10;                 /* 10^d   */
    seq_t pow10_mod_prev = 1 % mod_val;        /* 10^(d-1) mod n */
    seq_t pow10_mod_curr = 10 % mod_val;       /* 10^d   mod n */

    seq_t answer = 0;
    for (int d = 1; ; ++d) {
        /* r = (-C_{n-1} * 10^d) mod n  */
        seq_t r = (mod_val - (cur_mod * pow10_mod_curr) % mod_val) % mod_val;

        /* Minimum value with d digits */
        seq_t min_val = (d == 1) ? (seq_t)1 : pow10_val_prev;

        /* Compute the smallest m >= min_val with m ≡ r (mod n) */
        seq_t m;
        if (min_val <= r) {
            m = r;
            if (m == 0) m = mod_val;          /* ensure positive */
        } else {
            seq_t diff = min_val - r;         /* > 0 */
            seq_t k = (diff + mod_val - 1) / mod_val; /* ceil((min_val - r)/n) */
            m = r + k * mod_val;
        }

        if (m == 0) m = mod_val;               /* safety for r==0 case */

        /* Check digit length constraint */
        if (m < pow10_val_curr) {
            answer = m;
            break;
        }

        /* Prepare for next d */
        pow10_val_prev = pow10_val_curr;
        pow10_val_curr *= 10;
        pow10_mod_prev = pow10_mod_curr;
        pow10_mod_curr = (pow10_mod_prev * 10) % mod_val;
    }

    cache[n - 1] = answer;
    return answer;
}
