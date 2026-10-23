/*
 * A185954  G.f.: A(x) = exp( Sum_{n>=1} A163659(2n)*x^n/n ), where
 *          x*exp(Sum_{n>=1} A163659(n)*x^n/n) = S(x) is the g.f. of Stern's
 *          diatomic series (A002487).
 * https://oeis.org/A185954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = exp( Sum_{n>=1} A163659(2n)*x^n/n ) * Recurrence (derived
 *            from the generating‑function identity) * a(0) = 1 * for n >= 1:
 *            * R(n) = sum_{j∈{0,1,3,4,6,7}} c_j * a((n-j)/2) (j≤n, (n-j)
 *            even) * a(n) = R(n) + 2*a(n-1) + 2*a(n-4) - a(n-2) - a(n-3) -
 *            a(n-5) * where c = {1, 1, -2, -2, 1, 1} for j...
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values of a(n) */
static size_t cache_size = 0;        /* allocated size of cache */
static idx_t cache_max = 0;          /* maximum n for which a(n) is cached */

/* Helper: compute R(n) as described above */
static seq_t compute_R(idx_t n, seq_t *a)
{
    /* j-values and corresponding coefficients c_j */
    static const idx_t j_vals[]    = {0, 1, 3, 4, 6, 7};
    static const int   coeffs[]    = {1, 1, -2, -2, 1, 1};
    const size_t jcnt = sizeof(j_vals)/sizeof(j_vals[0]);

    seq_t R = 0;
    for (size_t k = 0; k < jcnt; ++k) {
        idx_t j  = j_vals[k];
        int   c  = coeffs[k];
        if (n < j) continue;
        if (((n - j) & 1) != 0) continue;          /* (n-j) even required */
        idx_t idx = (n - j) >> 1;                  /* (n-j)/2 */
        seq_t v = a[idx];
        if (c > 0) {
            /* c is 1 or 2 */
            if (c == 1) {
                R += v;
            } else { /* c == 2 */
                seq_t temp = v + v;    /* 2*v */
                R += temp;
            }
        } else { /* c is -1 or -2 */
            if (c == -1) {
                R -= v;
            } else { /* c == -2 */
                seq_t temp = v + v;    /* 2*v */
                R -= temp;
            }
        }
    }
    return R;
}

static void ensure_cache(idx_t n)
{
    if (cache_max >= n) return;          /* already computed */
    /* Expand cache if necessary */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();               /* out of memory */
        cache = tmp;
        cache_size = new_size;
    }
    /* Initialise a[0] if not already */
    if (cache_max == 0 && cache != NULL) {
        cache[0] = 1;
    }
    /* Compute values from cache_max+1 up to n */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        /* Compute R(k) using values already known */
        seq_t Rk = compute_R(k, cache);

        seq_t ak = Rk;

        if (k >= 1) {
            seq_t tmp = cache[k - 1];
            tmp += tmp;          /* 2 * a(k-1) */
            ak += tmp;
        }
        if (k >= 2) {
            ak -= cache[k - 2];
        }
        if (k >= 3) {
            ak -= cache[k - 3];
        }
        if (k >= 4) {
            seq_t tmp = cache[k - 4];
            tmp += tmp;          /* 2 * a(k-4) */
            ak += tmp;
        }
        if (k >= 5) {
            ak -= cache[k - 5];
        }

        cache[k] = ak;
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;               /* a(0) = 1 */
    ensure_cache(n);
    return cache[n];
}
