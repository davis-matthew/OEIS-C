/*
 * A118290  a(1) = 1. a(n) = number of terms among the sequence's first (n-1)
 *          terms which are divisible by the smallest prime dividing a(n-1),
 *          or which are divisible by 1 if a(n-1)= 1.
 * https://oeis.org/A118290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1. For n>1 let p be the smallest prime dividing a(n-1) (or 1 if a(n-1)=1).
   Then a(n) = #{1 ≤ i ≤ n-1 | a(i) divisible by p}.  */

/* ------------------------------------------------------------------ */

#include <stdlib.h>
#include <stdint.h>

/* small helper: trial‑division to find the smallest prime factor.  */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n <= 1)
        return 1;
    if ((n & 1) == 0)
        return 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0)
            return d;
    return n;              /* n is prime */
}

/* factorise n into its distinct prime factors.
   fac[] must be large enough to hold all factors (≤64 for 64‑bit n).  */
static void factor_unique(uint64_t n, uint32_t *fac, size_t *cnt)
{
    size_t i = 0;
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            fac[i++] = (uint32_t)d;
            while (n % d == 0)
                n /= d;
        }
    }
    if (n > 1)
        fac[i++] = (uint32_t)n;
    *cnt = i;
}

/* generic capacity growth used for the two dynamic arrays. */
static void ensure_capacity(size_t *cap, size_t need, void **ptr, size_t elem)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        if (new_cap < need)
            new_cap = need;
        void *p = realloc(*ptr, new_cap * elem);
        *ptr = p;
        *cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined, return 0 */
        return 0;

    /* static storage for previous terms and divisibility counts */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* counts of terms that are divisible by a given prime.
       counts[p] holds that count; index 1 is unused.          */
    static idx_t *cnt        = NULL;
    static size_t cnt_cap    = 0;

    /* allocate or grow array holding the sequence up to n */
    ensure_capacity(&cache_cap, (size_t)n, (void **)&cache, sizeof(seq_t));

    /* allocate or grow array holding prime divisibility counts
       we don't know the largest prime yet, so we grow on demand later. */
    if (cnt_cap < 2) {
        ensure_capacity(&cnt_cap, 2, (void **)&cnt, sizeof(idx_t));
        cnt[0] = cnt[1] = 0;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {   /* i = n-1 for new term */
        seq_t val;
        if (i == 0) {                                 /* a(1) */
            val = 1;
        } else {
            seq_t prev = cache[i - 1];
            if (prev == 1) {
                /* a(n) = n-1 when a(n-1)=1  */
                val = (seq_t)i;                       /* i = n-1 */
            } else {
                uint64_t spf = smallest_prime_factor((uint64_t)prev);
                /* make sure counts array covers spf */
                if ((size_t)spf >= cnt_cap)
                    ensure_capacity(&cnt_cap, (size_t)spf + 1, (void **)&cnt, sizeof(idx_t));
                val = (seq_t)cnt[spf];
            }
        }

        cache[i] = val;

        /* update counts for the new term */
        uint64_t v = (uint64_t)val;
        if (v > 1) {
            uint32_t fac[64];
            size_t fc;
            factor_unique(v, fac, &fc);
            for (size_t j = 0; j < fc; ++j) {
                uint32_t p = fac[j];
                if ((size_t)p >= cnt_cap)
                    ensure_capacity(&cnt_cap, (size_t)p + 1, (void **)&cnt, sizeof(idx_t));
                cnt[p] += 1;
            }
        }
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
