/*
 * A085048  a(2n-1) is odd, a(2n) = even, such that the difference of two
 *          terms with different parity is a prime.
 * https://oeis.org/A085048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: full, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A085048 (OEIS) by extending the signal
   iteratively:  a(2n-1) is odd, a(2n) is even.  For each new term
   choose the smallest integer larger than the previous one with the
   required parity and whose difference to every earlier term of
   opposite parity is a prime.  The sequence is finite, ending
   at a(23)=23. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

/* a(1) = 1, a(n) = 0 if n is beyond the finite sequence */
seq_t a(idx_t n)
{
    /* static cache for the computed terms */
    static seq_t *cache = NULL;   /* pointer to the array */
    static size_t cap   = 0;      /* allocated capacity   */
    static size_t len   = 0;      /* number of elements   */
    static bool built   = false;  /* true when no further term exists */

    /* initial allocation */
    if (!cache) {
        cap = 16;
        cache = mem_new(seq_t, cap);
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;                  /* a(1) = 1 */
        len = 1;
    }

    /* generate terms up to n (or until the sequence ends) */
    while (!built && len < (size_t)n) {
        seq_t last = cache[len - 1];
        bool want_odd = ((len + 1) & 1) != 0;        /* next index */

        /* search for the next term */
        const seq_t LIMIT = (seq_t)255;              /* safe upper bound */
        seq_t cand;
        bool found = false;

        for (cand = last + (seq_t)1; cand <= LIMIT; ++cand) {
            /* parity check */
            if (want_odd) {
                if (!(cand & (seq_t)1)) continue;   /* need odd */
            } else {
                if (cand & (seq_t)1) continue;       /* need even */
            }

            /* verify that difference to prior opposite‑parity terms is prime */
            bool ok = true;
            for (size_t k = 0; k < len; ++k) {
                if ((cache[k] & (seq_t)1) == (cand & (seq_t)1))
                    continue;                        /* same parity */

                seq_t diff = (cache[k] > cand) ?
                              cache[k] - cand : cand - cache[k];
                if (!is_prime_big(diff)) { ok = false; break; }
            }

            if (ok) {
                /* grow the cache if necessary */
                if (len == cap) {
                    size_t new_cap = cap * 2;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) return (seq_t)0;           /* allocation failure */
                    cache = tmp;
                    cap = new_cap;
                }
                cache[len++] = cand;
                found = true;
                break;
            }
        }

        /* if no candidate found before LIMIT, sequence ends */
        if (!found) built = true;
    }

    /* out-of-range request */
    if (n == 0 || (size_t)n > len)
        return (seq_t)0;

    return cache[n - 1];
}
