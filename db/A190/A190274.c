/*
 * A190274  Numbers n such that n' = p^2-1, with n = semiprime = p*q, n' is
 *          the arithmetic derivative of n. Also: semiprimes of the form
 *          p*(p^2-p-1).
 * https://oeis.org/A190274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A190274.
   A(n) is the nth semiprime of the form p * (p² – p – 1) where
   both p and  p² – p – 1 are primes.  The function works for any
   seq_t and idx_t provided by the build system.  It uses memoisation
   so that calls may come in arbitrary order. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
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

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t  *seq      = NULL;   /* array of computed terms   */
    static size_t seq_len  = 0;       /* number of terms stored    */
    static size_t seq_cap  = 0;       /* allocated capacity        */
    static uint64_t cand   = 3;       /* current integer to test   */

    while (seq_len < (size_t)n) {
        /* Find next prime p starting from cand */
        while (!is_prime_seq((seq_t)cand))
            cand++;

        uint64_t p = cand;
        uint64_t q = p * p - p - 1;          /* candidate factor */

        if (q >= 2 && is_prime_big((seq_t)q)) {
            seq_t val = (seq_t)p * (seq_t)q;   /* the semiprime     */

            if (seq_len == seq_cap) {
                size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
                seq_t *tmp = seq_realloc(seq, seq_cap, new_cap);
                if (!tmp) abort();            /* allocation failed */
                seq   = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = val;
        }
        cand++;  /* prepare for next candidate          */
    }
    return seq[n - 1];
}
