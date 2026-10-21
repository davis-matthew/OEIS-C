/*
 * A097854  Triangle read by rows: T(n,k) = number of Motzkin paths of length
 *          n and having abscissa of first return (i.e., first down step
 *          hitting the x-axis) equal to k (k>0); T(n,0)=1 (accounts for the
 *          paths consisting only of level steps).
 * https://oeis.org/A097854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k), where * r = floor((sqrt(8*n + 1) - 1) / 2) * k =
 *            n - r*(r+1)/2 (0‑based column) * T(r,0) = 1 * T(r,k) = m(r-k) *
 *            Σ_{j=0}^{k-2} m(j) for k > 0 * m(n) is the Motzkin number, with
 *            recurrence * m(0)=1, m(1)=1, * m(n) = ((2n+1)*m(n-1) +
 *            (3n-3)*m(n-2)) / (n+2) (n ≥ 2) * Row sums of the t...
 * Range:     u8 n <= 56, u16 n <= 134, u32 n <= 376, u64 n <= 1174,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- integer utilities ---------- */

static inline idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid != 0 && mid > n / mid) hi = mid - 1;
        else lo = mid;
    }
    return lo;
}

static inline idx_t tri_row(idx_t n) {
    /* compute floor((sqrt(8*n+1)-1)/2) */
    idx_t t = isqrt_u64(8ULL * n + 1);
    return (t - 1) / 2;
}

/* ---------- Motzkin numbers and prefix sums ---------- */

static seq_t   *motzkin = NULL;   /* m[0..max_m] */
static seq_t   *pref    = NULL;   /* prefix sums: pref[i] = Σ_{j=0..i} m(j) */
static idx_t     max_m   = 0;      /* largest index computed */

/* Ensure that m[0..up_to] and pref[0..up_to] are known. */
static void ensure_motzkin(idx_t up_to)
{
    if (up_to <= max_m) return;

    /* resize arrays */
    size_t newcap = (size_t)(up_to + 1);
    motzkin = (seq_t *)realloc(motzkin, newcap * sizeof(seq_t));
    pref    = (seq_t *)realloc(pref,    newcap * sizeof(seq_t));
    if (!motzkin || !pref) abort();

    /* initialise from scratch if necessary */
    if (max_m == 0) {
        motzkin[0] = (seq_t)1;
        pref[0]    = motzkin[0];
    }
    if (max_m == 1 && up_to >= 1) {
        /* m[1] is needed but may not have been set yet */
    }

    for (idx_t i = max_m + 1; i <= up_to; ++i) {
        if (i == 1) {
            motzkin[1] = (seq_t)1;
            pref[1]    = pref[0] + motzkin[1];
            continue;
        }

#if defined(__SIZEOF_INT128__) || defined(_MSC_VER)
        /* Use 128‑bit intermediates when seq_t is 64 bit or smaller. */
        unsigned __int128 term1 = (unsigned __int128)(2ULL * i + 1ULL) * (unsigned __int128)motzkin[i-1];
        unsigned __int128 term2 = (unsigned __int128)(3ULL * i - 3ULL) * (unsigned __int128)motzkin[i-2];
        motzkin[i] = (seq_t)((term1 + term2) / (i + 2));
#else
        /* Fallback to seq_t arithmetic (big‑integer builds). */
        seq_t term1 = ((seq_t)(2ULL * i + 1ULL)) * motzkin[i-1];
        seq_t term2 = ((seq_t)(3ULL * i - 3ULL)) * motzkin[i-2];
        motzkin[i]  = (term1 + term2) / (i + 2);
#endif
        pref[i] = pref[i-1] + motzkin[i];
    }

    max_m = up_to;
}

/* ---------- sequence a(n) ---------- */

seq_t a(idx_t n)
{
    idx_t r = tri_row(n);
    idx_t k = n - r * (r + 1) / 2;     /* column with 0‑based indexing */

    ensure_motzkin(r);                 /* need m up to r */

    if (k == 0) return (seq_t)1;       /* T(r,0) = 1 */

    seq_t sum = (k >= 2) ? pref[k - 2] : (seq_t)0; /* Σ_{j=0}^{k-2} m(j) */

#if defined(__SIZEOF_INT128__) || defined(_MSC_VER)
    unsigned __int128 prod = (unsigned __int128)motzkin[r - k] * (unsigned __int128)sum;
    return (seq_t)prod;
#else
    return motzkin[r - k] * sum;
#endif
}
