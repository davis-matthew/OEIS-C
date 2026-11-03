/*
 * A085949  Smallest multiple of prime(n) of the form r*prime(n-1) +
 *          s*prime(n+1) with r and s positive integers.
 * https://oeis.org/A085949
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 53 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest positive multiple of prime(n) that can be
 * expressed as r*prime(n-1) + s*prime(n+1) with r,s positive integers.
 * The function computes this value for n ≥ 2.  For smaller n it
 * returns 0.  Run‐time is O(prime(n)) and stays well below
 * two seconds for n up to about 10 000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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

/* fast modular exponentiation for 64‑bit integers */
static uint64_t mod_pow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1 % mod;
    a %= mod;
    while (e) {
        if (e & 1) res = (res * a) % mod;
        a = (a * a) % mod;
        e >>= 1;
    }
    return res;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n < 2)        /* defined only for n ≥ 2 */
        return 0;

    /* fetch consecutive primes */
    uint64_t p_prev_u = get_prime(n-1);
    uint64_t p_curr_u = get_prime(n);
    uint64_t p_next_u = get_prime(n+1);

    seq_t p_prev = (seq_t)p_prev_u;
    seq_t p_next = (seq_t)p_next_u;

    /* modular inverse of p_prev modulo p_curr (p_curr is prime) */
    uint64_t inv64 = mod_pow_uint64(p_prev_u % p_curr_u, p_curr_u-2, p_curr_u);
    seq_t best = 0;        /* best value found so far */
    bool first = true;

    /* search over s = 1 .. prime(n) */
    for (idx_t s = 1; s <= p_curr_u; ++s) {
        uint64_t s_u = (uint64_t)s;

        /* r ≡ -s * p_next * inv (mod p_curr) */
        uint64_t rhs_mod = (s_u * (p_next_u % p_curr_u)) % p_curr_u;
        uint64_t tmp = (p_curr_u - rhs_mod) % p_curr_u;
        uint64_t r0_u = (tmp * inv64) % p_curr_u;
        if (r0_u == 0) r0_u = p_curr_u;                /* r must be ≥ 1 */

        seq_t r0 = (seq_t)r0_u;
        seq_t cand = r0 * p_prev + (seq_t)s_u * p_next;

        if (first || cand < best) {
            best = cand;
            first = false;
        }
    }
    return best;
}
