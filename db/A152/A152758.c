/*
 * A152758  Numbers k such that the deficiency of k plus the number of proper
 *          divisors of k is not a prime number (see A152864).
 * https://oeis.org/A152758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth integer k such that * (2k – σ(k)) + (d(k) – 1) is
 *            NOT prime * where σ(k) is the sum of all divisors of k and d(k)
 *            is the * number of divisors of k. * This implementation searches
 *            increasing k, caches found terms, * and works for any seq_t and
 *            idx_t provided by the build system
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 4, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
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

static seq_t *terms = NULL;     /* cache of found terms */
static size_t terms_len = 0;    /* number of terms stored */
static size_t terms_cap = 0;    /* allocated capacity */

/* Ensure capacity for 'need' terms.  This is C++‑compatible. */
static void grow_terms(size_t need)
{
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, terms, new_cap);
    if (!tmp) abort();          /* out of memory */
    terms = tmp;
    terms_cap = new_cap;
}

/* Compute (2k – σ(k)) + (d(k) – 1) for a given k. */
static seq_t compute_value(idx_t k)
{
    idx_t n = k;               /* use a copy for factorisation */
    seq_t sigma = (seq_t)1;
    seq_t tau   = (seq_t)1;

    for (idx_t p = 2; p * p <= n; ++p)
    {
        if (n % p != 0) continue;
        idx_t cnt = 0;
        while (n % p == 0)
        {
            n /= p;
            ++cnt;
        }
        /* Compute σ-part: (p^(cnt+1)-1)/(p-1) */
        seq_t num = (seq_t)1;
        seq_t pseq = (seq_t)p;
        for (idx_t i = 0; i <= cnt; ++i) num *= pseq;
        num -= (seq_t)1;
        seq_t denom = (seq_t)(p - 1);
        seq_t sigma_part = num / denom;
        sigma *= sigma_part;
        tau   *= (seq_t)(cnt + 1);
    }
    if (n > 1)   /* remaining prime factor */
    {
        seq_t pseq = (seq_t)n;
        sigma *= (pseq + (seq_t)1);
        tau   *= (seq_t)2;
    }

    seq_t two_k = ((seq_t)2 * (seq_t)k);
    seq_t def   = two_k - sigma;          /* deficiency */
    seq_t val   = def + tau - (seq_t)1;   /* target value */
    return val;
}

/* Main function requested by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety for calls below the offset */

    /* Grow cache until we have at least n terms */
    while (terms_len < n)
    {
        /* Start searching from the next integer after the last found term,
           or from 1 if this is the first term. */
        idx_t k = terms_len ? (idx_t)(terms[terms_len - 1] + (seq_t)1) : (idx_t)1;
        bool found = false;
        while (!found)
        {
            seq_t val = compute_value(k);
            bool not_prime;
            if (val <= (seq_t)1)          /* 0 or 1 are not prime */
                not_prime = true;
            else
                not_prime = !is_prime_big(val);

            if (not_prime)
            {
                grow_terms(terms_len + 1);
                terms[terms_len++] = (seq_t)k;
                found = true;
            }
            ++k;
        }
    }
    return terms[n - 1];
}
