/*
 * A103226  Moebius function mu(n+ki) defined for the Gaussian integers. The
 *          table begins with n=k=0 and is read by antidiagonals.
 * https://oeis.org/A103226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, sign, tabl
 * Formula:   a(n) = μ(a+bi) where (a,b) is the Gaussian integer with * a+b =
 *            s, where s is the smallest integer such that (s+1)(s+2)/2 ≥ n. *
 *            The index n starts at 1 (offset 1). μ is the Möbius function in
 *            * the ring ℤ[i] (Gaussian integers). The sequence is read by *
 *            antidiagonals of the Gaussian lattice. Th...
 * Range:     i8 n <= 108, i16 n <= 108, i32 n <= 108, i64 n <= 108,
 *            i128 n <= 108, big any n
 * Helpers:   divide_gaussian
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/divide_gaussian.h"

/* --- Gaussian prime table ------------------------------------------- */

#define MAX_GAUSS_PRIMES  12000
#define MAX_PRIME        40000

static int gp_a[MAX_GAUSS_PRIMES];
static int gp_b[MAX_GAUSS_PRIMES];
static int gp_cnt = 0;
static bool primes_ready = false;

/* integer square‑root (floor) */
static inline int int_sqrt(int x) {
    int r = 1;
    while ((long long)r * r <= x) ++r;
    return r - 1;
}

/* Build all Gaussian primes up to the required norm. */
static void init_gauss_primes(void) {
    if (primes_ready) return;
    primes_ready = true;

    static bool is_prime[MAX_PRIME + 1];
    for (int i = 0; i <= MAX_PRIME; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= MAX_PRIME; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= MAX_PRIME; j += i)
                is_prime[j] = false;

    gp_cnt = 0;
    for (int p = 2; p <= MAX_PRIME; ++p)
        if (is_prime[p]) {
            if (p == 2) {                 /* 1 + i  and 1 – i */
                gp_a[gp_cnt] = 1;  gp_b[gp_cnt] = 1;  gp_cnt++;
                gp_a[gp_cnt] = 1;  gp_b[gp_cnt] = -1; gp_cnt++;
            } else if (p % 4 == 3) {       /* p remains prime in ℤ[i] */
                gp_a[gp_cnt] = p; gp_b[gp_cnt] = 0; gp_cnt++;
            } else {                       /* p ≡ 1 (mod 4), split */
                int amax = int_sqrt(p);
                for (int a = 1; a <= amax; ++a) {
                    int b2 = p - a * a;
                    if (b2 <= 0) continue;
                    int b = int_sqrt(b2);
                    if (b * b == b2) {    /* found representation */
                        gp_a[gp_cnt] = a;  gp_b[gp_cnt] = b;  gp_cnt++;
                        gp_a[gp_cnt] = a;  gp_b[gp_cnt] = -b; gp_cnt++;
                        break;
                    }
                }
                /* it is guaranteed that a representation exists */
            }
        }
}

/* Möbius function over Gaussian integers. */
static seq_t mu_gaussian(int a, int b) {
    if (a == 0 && b == 0) return 0;
    init_gauss_primes();

    int aa = a, bb = b;
    int primes_cnt = 0;

    for (int idx = 0; idx < gp_cnt; ++idx) {
        int pa = gp_a[idx], pb = gp_b[idx];
        int exp = 0;
        while (true) {
            int qa, qb;
            if (!divide_gaussian(aa, bb, pa, pb, &qa, &qb))
                break;
            if (exp) return 0;   /* squared prime factor */
            exp = 1;
            ++primes_cnt;
            aa = qa;
            bb = qb;
            if (aa == 0 && bb == 0) break;          /* fully factored */
        }
        if (aa == 0 && bb == 0) break;
    }

    /* If a unit (norm 1) remains, do not count it as a prime factor. */
    unsigned long long norm = (unsigned long long)aa * aa +
                              (unsigned long long)bb * bb;
    if (norm > 1) ++primes_cnt;

    return (primes_cnt & 1) ? (seq_t)-1 : (seq_t)1;
}

/* Triangular number T(s) = (s+1)(s+2)/2 */
static inline idx_t triangular(idx_t s) {
    return (s + 1) * (s + 2) / 2;
}

/* Find the diagonal number s such that T(s-1) < n ≤ T(s) */
static inline idx_t find_diagonal(idx_t n) {
    idx_t low = 0, high = 1;
    while (triangular(high) < n) high <<= 1;   /* grow high */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* --- public function --------------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* outside offset, undefined anyway */

    idx_t s = find_diagonal(n);          /* diagonal number */
    idx_t prev_terms = s * (s + 1) / 2;  /* T(s-1) */
    idx_t pos = n - prev_terms - 1;      /* 0‑based position on the diagonal */

    int k = (int)pos;                   /* imaginary part */
    int a_real = (int)(s - pos);        /* real part */

    return mu_gaussian(a_real, k);
}
