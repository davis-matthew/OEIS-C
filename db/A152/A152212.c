/*
 * A152212  Numbers k such that sigma_0(k) divides (k*sigma_1(k) -
 *          sigma_2(k)).
 * https://oeis.org/A152212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 291, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A152212 – Numbers k such that σ₀(k) | (k·σ₁(k) – σ₂(k)).
    The function generates the required k by successive trial, caches
    results, and returns the nth term (1‑based).  No external utilities
    are required. */

#include <stdlib.h>

/* ------------------------------------------------------------ */
/* Basic helpers ------------------------------------------------ */

/* fast exponentiation: base^exp  (exp fits in idx_t)             */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* ------------------------------------------------------------ */
/* Small‑prime‑factor array ------------------------------------ */

static idx_t *spf = NULL;        /* smallest prime factor of i   */
static idx_t spf_limit = 0;      /* spf is valid up to this      */

/* Build spf[0..limit] inclusive.  Simple sieve, recomputed each
   time when the limit is increased.  For the problem size this
   overhead is negligible.                                            */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;
    if (spf) free(spf);
    idx_t *new_spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!new_spf) abort();
    for (idx_t i = 0; i <= limit; ++i) new_spf[i] = 0;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!new_spf[i]) {              /* i is prime */
            new_spf[i] = i;
            for (idx_t j = i * i; j <= limit; j += i)
                if (!new_spf[j]) new_spf[j] = i;
        }
    }
    for (idx_t i = 2; i <= limit; ++i)
        if (!new_spf[i]) new_spf[i] = i;
    spf = new_spf;
    spf_limit = limit;
}

/* ------------------------------------------------------------ */
/* Criterion check -------------------------------------------- */

static int satisfies(idx_t k)
{
    if (k == 1) return 1;            /* σ₀(1)=σ₁(1)=σ₂(1)=1 → true */

    /* ensure spf covers k  */
    if (k > spf_limit) ensure_spf(k);

    seq_t sigma0 = 1, sigma1 = 1, sigma2 = 1;
    idx_t n = k;
    while (n > 1) {
        idx_t p = spf[n];
        int e = 0;
        while (n % p == 0) { n /= p; ++e; }

        /* σ₀ contribution */
        sigma0 *= (seq_t)(e + 1);

        /* σ₁ contribution: (p^{e+1}-1)/(p-1) */
        seq_t p_pow_e1 = pow_seq((seq_t)p, e + 1);
        seq_t sigma1_factor = (p_pow_e1 - 1) / ((seq_t)p - 1);
        sigma1 *= sigma1_factor;

        /* σ₂ contribution: (p^{2(e+1)}-1)/(p²-1) */
        seq_t p_pow_2e2 = pow_seq((seq_t)p, 2 * (e + 1));
        seq_t p_sq = (seq_t)p * (seq_t)p;
        seq_t sigma2_factor = (p_pow_2e2 - 1) / (p_sq - 1);
        sigma2 *= sigma2_factor;
    }

    seq_t k_seq = (seq_t)k;
    seq_t lhs = k_seq * sigma1 - sigma2;
    return (lhs % sigma0) == 0;
}

/* ------------------------------------------------------------ */
/* Sequence cache --------------------------------------------- */

static seq_t *cache = NULL;   /* cached terms */
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t test_k = 1;      /* next integer to test */

/* Grow the cache array if needed. */
static void ensure_cache(void)
{
    if (cache_len < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------ */
/* Exported function ------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but safe */

    /* return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* generate terms until we have n of them */
    while (cache_len < n) {
        if (satisfies(test_k)) {
            ensure_cache();
            cache[cache_len++] = (seq_t)test_k;
        }
        ++test_k;
    }
    return cache[n - 1];
}
