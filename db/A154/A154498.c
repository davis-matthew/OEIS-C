/*
 * A154498  Sum of any 3 consecutive numbers is prime, a(1)=41,a(2)=43.
 * https://oeis.org/A154498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function generates OEIS sequence A154498.
   The sequence is defined as the list of primes a(n) (n≥1) 
   such that a(n) is the smallest prime greater than a(n‑1)
   with the property that a(n‑2)+a(n‑1)+a(n) is also prime.
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>          /* for uint64_t (idx_t) */
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* cache[0] == a(1), cache[1] == a(2), ... */
    static seq_t *cache = NULL;      /* dynamic array of computed terms */
    static idx_t cached = 0;         /* number of terms stored */
    static size_t cap = 0;           /* allocated capacity */

    if (n == 0)                      /* defined only for n>=1 */
        return 0;

    /* enlarge cache if necessary */
    if (cached < n) {
        if (cap < (size_t)n) {
            cap = (size_t)n;
            cache = mem_resize(seq_t, cache, cap);
            if (!cache) abort();    /* allocation failed */
        }

        for (idx_t i = cached; i < n; ++i) {
            seq_t val;
            if (i == 0) {            /* a(1) */
                val = 41;
            } else if (i == 1) {     /* a(2) */
                val = 43;
            } else {
                seq_t prev1 = cache[i - 1];   /* a(n‑1) */
                seq_t prev2 = cache[i - 2];   /* a(n‑2) */

                seq_t cand = prev1 + 2;       /* next odd number */
                for (;;) {
                    if (!is_prime_big(cand)) {
                        cand += 2;            /* skip composites */
                        continue;
                    }
                    seq_t sum = prev2 + prev1 + cand;
                    if (is_prime_big(sum)) {
                        val = cand;
                        break;
                    }
                    cand += 2;                /* next odd prime candidate */
                }
            }
            cache[i] = val;
        }
        cached = n;
    }

    return cache[n - 1];   /* return a(n) */
}
