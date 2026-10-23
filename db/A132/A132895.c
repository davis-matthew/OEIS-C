/*
 * A132895  Even numbers for which all divisors, with the exception of 1 and
 *          2, are isolated. A positive divisor, k, of n is isolated if
 *          neither (k-1) nor (k+1) divides n.
 * https://oeis.org/A132895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A132895 is computed by searching even numbers n such that all
 *            divisors d > 2 of n satisfy that neither (d-1) nor (d+1) divides
 *            n
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *a_cache = NULL;      /* array of computed sequence terms   */
static idx_t  a_cache_len = 0;     /* number of terms stored              */
static idx_t  a_cache_cap = 0;     /* current capacity of a_cache         */
static idx_t  last_checked_even = 0;  /* last even number examined          */

/* Test whether an even number m has the required property. */
static bool candidate_ok(seq_t m)
{
    const seq_t TWO = 2;
    seq_t temp = m;
    seq_t primes[64];
    unsigned int exps[64];
    int nf = 0;          /* number of distinct prime factors   */
    unsigned int exp;

    /* factor out 2^exp */
    exp = 0;
    while (temp % TWO == 0) {
        exp++;
        temp /= TWO;
    }
    if (exp) {
        primes[nf] = TWO;
        exps[nf++] = exp;
    }

    /* factor odd primes */
    for (idx_t p = 3; ; p += 2) {
        seq_t p_seq = (seq_t)p;
        seq_t p_sq = p_seq * p_seq;
        if (p_sq > temp) break;
        exp = 0;
        while (temp % p_seq == 0) {
            exp++;
            temp /= p_seq;
        }
        if (exp) {
            if (nf >= 64) return false;  /* safety guard */
            primes[nf] = p_seq;
            exps[nf++] = exp;
        }
    }
    if (temp > 1) {   /* remaining prime factor */
        if (nf < 64) {
            primes[nf] = temp;
            exps[nf++] = 1;
        }
    }

    /* generate all divisors of m */
    seq_t divs[256];
    size_t div_count = 1;
    divs[0] = 1;
    for (int i = 0; i < nf; ++i) {
        seq_t p_seq = primes[i];
        unsigned int eexp = exps[i];
        seq_t pow = 1;
        size_t prev = div_count;
        for (unsigned int e = 1; e <= eexp; ++e) {
            pow *= p_seq;
            for (size_t j = 0; j < prev; ++j) {
                divs[div_count++] = divs[j] * pow;
            }
        }
    }

    /* check isolation condition for each divisor > 2 */
    for (size_t k = 0; k < div_count; ++k) {
        seq_t d = divs[k];
        if (d <= 2) continue;
        seq_t d_minus = d - 1;
        seq_t d_plus  = d + 1;
        if (m % d_minus == 0 || m % d_plus == 0) {
            return false;
        }
    }
    return true;
}

/* Public function implementing the OEIS sequence A132895. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* below offset, value irrelevant */

    /* initialize cache on first call */
    if (!a_cache) {
        a_cache_cap = 8;
        a_cache = seq_alloc(a_cache_cap);
        a_cache_len = 0;
        last_checked_even = 0;
    }

    /* generate terms until we reach the requested index */
    while (a_cache_len < n) {
        idx_t e = (last_checked_even == 0 ? 2 : last_checked_even + 2);
        if (candidate_ok((seq_t)e)) {
            if (a_cache_len + 1 > a_cache_cap) {
                idx_t new_cap = a_cache_cap == 0 ? 8 : a_cache_cap * 2;
                a_cache = seq_realloc(a_cache, a_cache_cap, new_cap);
                a_cache_cap = new_cap;
            }
            a_cache[a_cache_len++] = (seq_t)e;
        }
        last_checked_even = e;
    }

    return a_cache[n - 1];
}
