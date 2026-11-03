/*
 * A091210  Indices of primes that are composite GF(2)[X]-polynomials.
 * https://oeis.org/A091210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th prime whose binary polynomial is
 *            composite over GF(2)[X]
 * Range:     u8 n <= 27, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-03)
 * Helpers:   bit_length, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binary polynomial of an integer p is the polynomial obtained
 * from the binary expansion of p: p = Σ bi·x^i,  bi∈{0,1}.
 * The polynomial is composite if it has a non‑trivial factor over GF(2).
 * The implementation tests for a linear factor (parity of the ones),
 * otherwise it tries all monic factors up to degree floor(deg/2) and
 * checks divisibility by polynomial long division.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/bit_length.h"
#include "../utils/mem.h"

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

static inline bool composite_poly(seq_t f)
{
    int deg = bit_length(f) - 1;               /* degree of the polynomial */
    if (deg <= 1) return false;                 /* linear or constant → irreducible */

    /* linear factor:  (x+1) divides f iff sum of coefficients is even */
    /* compute parity of ones in f */
    unsigned long long val = (unsigned long long)f;          /* p fits in 64 bits */
    if ((__builtin_parityll(val) == 0)) return true;

    /* try other monic factors of degree 1 .. deg/2 */
    for (int d = 1; d <= deg / 2; ++d)
    {
        seq_t base = ((seq_t)1 << d);           /* leading coefficient 1 at degree d */
        int limit = 1 << d;                    /* number of possibilities for lower bits */

        for (int g_low = 0; g_low < limit; ++g_low)
        {
            seq_t g = base | ((seq_t)g_low);

            /* simple long division: f ÷ g */
            seq_t rem = f;
            int deg_rem = deg;

            while (deg_rem >= d && rem != 0)
            {
                int shift = deg_rem - d;
                rem ^= (g << shift);
                deg_rem = bit_length(rem) - 1;
            }

            if (rem == 0) return true;          /* g divides f */
        }
    }

    return false;                                 /* irreducible */
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* cached indices of qualifying primes */
    static size_t cache_cap = 0;        /* allocated capacity of cache     */
    static size_t cache_len = 0;        /* number of indices stored        */
    static idx_t processed_k = 0;       /* largest prime index processed   */

    if (n == 0) return 0;              /* not used; offset is 1           */

    while (cache_len < n)               /* extend cache until n entries    */
    {
        idx_t k = processed_k + 1;
        seq_t p = get_prime(k);         /* fetch k-th prime                */

        if (composite_poly(p))          /* test prime for property        */
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)                /* memory overflow – abort      */
                    exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;   /* store index of qualifying prime */
        }

        processed_k = k;
    }

    return cache[n - 1];
}
