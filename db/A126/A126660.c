/*
 * A126660  Array read by antidiagonals: A(k,n) = smallest odd prime q such
 *          that pq+2k is prime, where p is the n-th odd prime, or 0 if no
 *          such prime exists.
 * https://oeis.org/A126660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = array read by antidiagonals of * A(k,n) = smallest odd
 *            prime q such that p*q+2k is prime, * where p is the n-th odd
 *            prime. If no such prime exists, * a(n) = 0. * The sequence is
 *            defined for n >= 1 with offset 1
 * Range:     u8 n <= 16, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 104 terms through n = 104 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

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

/* Upper bound for q to avoid infinite search. */
#define Q_LIMIT 200000

static seq_t
solve_entry(idx_t k, idx_t col)
{
    /* p = nth odd prime at column 'col' (1‑based) */
    seq_t p = (seq_t) get_prime(col + 1);  /* col=1 returns the 2nd prime, 3 */

    /* Search for smallest odd prime q such that p*q + 2k is prime. */
    for (idx_t q_index = 2; /* start at 3 */; ++q_index) {
        seq_t q = (seq_t) get_prime(q_index);   /* q increases over odd primes */
        if (q > (seq_t) Q_LIMIT)   /* give up if q exceeds bound */
            break;

        seq_t val = p * q + ((seq_t)2 * (seq_t)k);
        if (is_prime_big(val))
            return q;
    }

    return 0;
}

seq_t a(idx_t n)
{
    /* Find diagonal s such that the cumulative count up to that
     * diagonal just exceeds n.  The cumulative count up to
     * diagonal s (sum of indices = s) is T_{s-1} = (s-1)*s/2. */
    idx_t m = 0;          /* will end up as the smallest integer with
                            T_m >= n */
    idx_t cumulative = 0;
    while (cumulative < n) {
        ++m;
        cumulative += m;
    }

    idx_t s = m + 1;                         /* diagonal sum k + n = s   */
    idx_t T_prev = ((m - 1) * m) / 2;        /* T_{s-2} = cumulative of previous diag */
    idx_t offset = n - T_prev - 1;

    idx_t k = m - offset;                    /* row index   (1‑based)   */
    idx_t col = offset + 1;                  /* column index (1‑based)   */

    return solve_entry(k, col);
}
