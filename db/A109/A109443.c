/*
 * A109443  Cumulative sum of largest prime power <= n.
 * https://oeis.org/A109443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} A031218(i) (largest prime power ≤ i)
 * Range:     u8 n <= 22, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code below works for both the C and C++ builds.  It keeps a globally
   cached SPF array, an array of the largest prime power seen so far,
   and the cumulative sum.  All arrays are grown lazily when a larger n
   is requested.  The use of *realloc* with explicit casts makes the code
   valid under both C (where seq_t is a plain struct) and C++ (where
   seq_t may be a big‑int class).  The function is thread‑unsafe but
   satisfies the function‑storage requirements of the OEIS tests. */

#include <stdlib.h>
#include <stdint.h>

/* forward declaration of the helper to build the SPF array */
static void compute_spf(idx_t n, uint32_t **spf_ptr, idx_t *cap);

/* static state */
static idx_t        max_computed = 0;   /* highest n already processed   */
static seq_t        *cum          = NULL; /* cumulative sums  */
static seq_t        *best         = NULL; /* best prime power up to i  */
static uint32_t     *spf          = NULL; /* smallest prime factor table */
static idx_t        spf_cap       = 0;    /* allocated size of spf */

/* ----------------------------------------------------------- */
/* Build smallest‑prime‑factor array up to n (inclusive).  If
   the array is already large enough, this does nothing.            */
static void compute_spf(idx_t n, uint32_t **spf_ptr, idx_t *cap)
{
    if (*cap < n + 1) {
        *cap = n + 1;
        *spf_ptr = (uint32_t*)realloc(*spf_ptr, *cap * sizeof(uint32_t));
    }

    uint32_t *s = *spf_ptr;

    /* initialise: every number is its own smallest prime factor */
    for (idx_t i = 0; i <= n; ++i)
        s[i] = (uint32_t)i;

    /* sieve */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (s[i] == (uint32_t)i) {            /* i is prime */
            for (idx_t j = i * i; j <= n; j += i)
                if (s[j] == (uint32_t)j)
                    s[j] = (uint32_t)i;
        }
    }
}

/* ----------------------------------------------------------- */
/* Compute a(n) – the cumulative sum of the largest prime power
   ≤ i for 1 ≤ i ≤ n.  The function can be called with
   arbitrary n (>=1) in any order.                               */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;     /* undefined for n=0, but harmless */

    /* enlarge state if needed */
    if (n > max_computed) {
        /* Build SPF up to n */
        compute_spf(n, &spf, &spf_cap);

        /* allocate or grow the arrays */
        idx_t new_cap = n + 1;
        cum  = (seq_t*)realloc(cum,  new_cap * sizeof(seq_t));
        best = (seq_t*)realloc(best, new_cap * sizeof(seq_t));

        /* starting values for the extension loop */
        seq_t last_best = (max_computed == 0 ? (seq_t)0 : best[max_computed]);
        seq_t prev_cum  = (max_computed == 0 ? (seq_t)0 : cum[max_computed]);

        /* fill new entries */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t cur_best;

            if (i == 1) {
                cur_best = (seq_t)1;          /* 1 is considered a prime power */
            } else {
                uint32_t p  = spf[(size_t)i];
                uint32_t tmp = (uint32_t)i;
                while (tmp % p == 0)
                    tmp /= p;
                if (tmp == 1)                 /* i itself is a prime power */
                    cur_best = (seq_t)i;
                else
                    cur_best = last_best;     /* keep previous best */
            }

            best[i] = cur_best;
            cum[i]  = prev_cum + cur_best;
            last_best = cur_best;
            prev_cum  = cum[i];
        }

        max_computed = n;
    }

    return cum[n];
}
