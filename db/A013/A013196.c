/*
 * A013196  Expansion of e.g.f. log(sec(x) + arctan(x)).
 * https://oeis.org/A013196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A013196.
   a(n) are the coefficients of the exponential generating function
   log(sec(x)+arctan(x)), i.e.   log(sec(x)+arctan(x)) = Σ a(n) xⁿ/n!.
   The recurrence below uses only integer arithmetic. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Forward declaration of required types. */
extern seq_t a(idx_t n);

/* ---------- internal helper functions ---------- */

#ifdef __cplusplus
/* Dynamic storage using std::vector. */
static std::vector<seq_t> fact, sec_coeff, g_coeff, a_coeff;
static idx_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;
    /* Resize all vectors to hold up to n (inclusive). */
    fact.resize(n + 1);
    sec_coeff.resize(n + 1);
    g_coeff.resize(n + 1);
    a_coeff.resize(n + 1);

    /* Compute from the current cache_size up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* factorial */
        if (i == 0) {
            fact[0] = (seq_t)1;
        } else {
            fact[i] = fact[i - 1] * (seq_t)i;
        }

        /* f_atan[i] = coefficient of arctan in the e.g.f. */
        seq_t atan_coef;
        if ((i & 1ULL) == 1ULL) {
            /* odd i : (-1)^((i-1)/2) * (i-1)! */
            seq_t sign = (((i >> 1) & 1ULL) ? (seq_t)-1 : (seq_t)1);
            atan_coef = sign * fact[i - 1];
        } else {
            atan_coef = 0;
        }

        /* sec_coeff[i] : coefficients of sec(x) in the e.g.f. */
        if (i == 0) {
            sec_coeff[0] = (seq_t)1;
        } else {
            seq_t sum = 0;
            seq_t bin = 1; /* C(i,0) */
            for (idx_t k = 0; k < i; ++k) {
                idx_t m = i - k;
                if ((m & 1ULL) == 0ULL) {
                    /* cos coefficient d_m = (-1)^(m/2) */
                    seq_t d = (((m >> 1) & 1ULL) ? (seq_t)-1 : (seq_t)1);
                    sum += bin * sec_coeff[k] * d;
                }
                if (k != i - 1) {
                    /* update bin to C(i, k+1) = C(i,k)*(i-k)/(k+1) */
                    bin = bin * (seq_t)(i - k) / (seq_t)(k + 1);
                }
            }
            sec_coeff[i] = -sum;
        }

        /* g_coeff[i] = coefficient of sec(x)+arctan(x) in the e.g.f. */
        g_coeff[i] = sec_coeff[i] + atan_coef;

        /* a_coeff[i] : required sequence values. */
        if (i == 0) {
            a_coeff[0] = 0;
            continue;
        }
        seq_t sum_a = 0;
        seq_t bin_a = 1; /* C(i-1,0) */
        for (idx_t k = 1; k < i; ++k) {
            bin_a = bin_a * (seq_t)(i - k) / (seq_t)k; /* C(i-1,k) */
            sum_a += bin_a * g_coeff[k] * a_coeff[i - k];
        }
        a_coeff[i] = g_coeff[i] - sum_a;
    }

    cache_size = n + 1;
}
#else /* C implementation with fixed maximum size */

/* Arbitrary maximum cache size for C compilation. */
#define MAX_CACHE 12000
static seq_t fact[MAX_CACHE];
static seq_t sec_coeff[MAX_CACHE];
static seq_t g_coeff[MAX_CACHE];
static seq_t a_coeff[MAX_CACHE];
static idx_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;
    if (n >= MAX_CACHE) return; /* silently clamp */

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            fact[0] = (seq_t)1;
            sec_coeff[0] = (seq_t)1;
            a_coeff[0] = 0;
        } else {
            fact[i] = fact[i - 1] * (seq_t)i;
        }

        /* atan coefficient */
        seq_t atan_coef;
        if ((i & 1ULL) == 1ULL) {
            seq_t sign = (((i >> 1) & 1ULL) ? (seq_t)-1 : (seq_t)1);
            atan_coef = sign * fact[i - 1];
        } else {
            atan_coef = 0;
        }

        /* sec coefficient */
        if (i == 0) {
            /* already set */
        } else {
            seq_t sum = 0;
            seq_t bin = 1; /* C(i,0) */
            for (idx_t k = 0; k < i; ++k) {
                idx_t m = i - k;
                if ((m & 1ULL) == 0ULL) {
                    seq_t d = (((m >> 1) & 1ULL) ? (seq_t)-1 : (seq_t)1);
                    sum += bin * sec_coeff[k] * d;
                }
                if (k != i - 1) {
                    bin = bin * (seq_t)(i - k) / (seq_t)(k + 1);
                }
            }
            sec_coeff[i] = -sum;
        }

        g_coeff[i] = sec_coeff[i] + atan_coef;

        if (i == 0) continue;
        seq_t sum_a = 0;
        seq_t bin_a = 1; /* C(i-1,0) */
        for (idx_t k = 1; k < i; ++k) {
            bin_a = bin_a * (seq_t)(i - k) / (seq_t)k; /* C(i-1,k) */
            sum_a += bin_a * g_coeff[k] * a_coeff[i - k];
        }
        a_coeff[i] = g_coeff[i] - sum_a;
    }

    cache_size = n + 1;
}
#endif /* __cplusplus */

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    ensure_cache(n);
#ifdef __cplusplus
    return a_coeff[(size_t)n];
#else
    return a_coeff[n];
#endif
}
