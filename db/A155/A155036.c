/*
 * A155036  Primes p = prime(c) with index c a composite such that the
 *          concatenation c//p is a prime.
 * https://oeis.org/A155036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = prime(c) where c is composite and concat(c, prime(c)) is
 *            prime
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <stddef.h>
#include "../utils/is_prime.h"
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

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* --------- static data for memoisation --------- */
static seq_t *seq_terms   = NULL;   /* array of computed terms */
static size_t terms_len   = 0;      /* number of terms stored */
static size_t terms_cap   = 0;      /* current capacity of seq_terms */

static idx_t last_checked_c = 0;    /* last composite index examined */

/* enlarge seq_terms array if needed */
static void ensure_terms_cap(size_t needed)
{
    if (terms_cap >= needed)
        return;
    size_t new_cap = (terms_cap == 0) ? 8 : terms_cap * 2;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_terms = mem_resize(seq_t, seq_terms, new_cap);
    terms_cap = new_cap;
}

/* store a new term */
static void add_term(seq_t val)
{
    ensure_terms_cap(terms_len + 1);
    seq_terms[terms_len++] = val;
}

/* extend computation until we have at least target terms */
static void compute_until(idx_t target)
{
    if (terms_len >= target)
        return;

    idx_t c = last_checked_c;
    while (terms_len < target) {
        ++c;  /* next index */

        /* skip 0,1 and all prime indices */
        if (c > 1 && !is_prime(c)) {
            /* p = prime(c) */
            uint32_t p_uint32 = get_prime(c);
            seq_t p = (seq_t)p_uint32;

            /* concat(c, p) */
            seq_t concatenated = concat_parts(c, (idx_t)p_uint32);

            if (is_prime_big(concatenated)) {
                add_term(p);
            }
        }
    }

    last_checked_c = c;
}

/* ----------------------------------------------------- */
/* public sequence function required by the exercise    */
/* ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined, safety */
        return (seq_t)0;
    compute_until(n);
    return seq_terms[n - 1];
}
