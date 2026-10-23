/*
 * A127702  Least value of x*y giving Max ( sigma(x*y) : 1<=x<=n, 1<=y<=n ).
 * https://oeis.org/A127702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal product x*y (1≤x,y≤n) giving maximal σ(x*y);
 *            tie‑broken by the smallest product. Computation is O(N²) for N =
 *            10000 and works for any seq_t, idx_t
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Dimensions ------------------------------------------------------------- */
#define N_MAX 10000
/* maximum number of distinct primes in a factorisation of a number ≤ N_MAX */
#define MAX_FACTORS 6
/* maximum prime <= N_MAX (fits in uint16_t) */
#define PRIME_MAX 10000

/* ----------------------------------------------------------------------- */
/* Factorisation data structures ------------------------------------------- */
typedef struct {
    uint16_t prime[MAX_FACTORS];
    uint8_t  exp[MAX_FACTORS];
    uint8_t  cnt;      /* number of distinct primes */
} factor_t;

static factor_t factor[N_MAX + 1];   /* factorisation of 1..N_MAX */

/* ----------------------------------------------------------------------- */
/* Prime sieve for 1..N_MAX ----------------------------------------------- */
static uint16_t primes[1229];            /* sqrt(10000) < 1229 primes */
static size_t  prime_cnt = 0;

/* precomputes primes up to N_MAX and factorisations of 1..N_MAX */
static void init_primes_and_factors(void) {
    static bool done = false;
    if (done) return;
    done = true;

    /* sieve */
    unsigned char mark[N_MAX + 1] = {0};
    for (int i = 2; i * i <= N_MAX; ++i) {
        if (!mark[i]) {
            for (int j = i * i; j <= N_MAX; j += i) mark[j] = 1;
        }
    }
    prime_cnt = 0;
    for (int i = 2; i <= N_MAX; ++i) if (!mark[i]) primes[prime_cnt++] = (uint16_t)i;

    /* factorisation of 1..N_MAX */
    factor[1].cnt = 0; /* 1 has no prime factors */
    for (int n = 2; n <= N_MAX; ++n) {
        int temp = n;
        int c = 0;
        for (size_t pi = 0; pi < prime_cnt && (int)primes[pi] * (int)primes[pi] <= temp; ++pi) {
            if (temp % primes[pi] == 0) {
                int e = 0;
                while (temp % primes[pi] == 0) {
                    temp /= primes[pi];
                    ++e;
                }
                factor[n].prime[c] = primes[pi];
                factor[n].exp[c]  = (uint8_t)e;
                ++c;
            }
        }
        if (temp > 1) { /* remaining prime > sqrt(n) */
            factor[n].prime[c] = (uint16_t)temp;
            factor[n].exp[c]  = 1;
            ++c;
        }
        factor[n].cnt = c;
    }
}

/* ----------------------------------------------------------------------- */
/* Merge factorisations of two numbers and compute σ(product) ------------ */
static uint64_t sigma_of_product(uint32_t x, uint32_t y) {
    const factor_t *fx = &factor[x];
    const factor_t *fy = &factor[y];
    uint64_t sigma = 1;
    int ix = 0, iy = 0;
    while (ix < fx->cnt || iy < fy->cnt) {
        uint16_t px = (ix < fx->cnt) ? fx->prime[ix] : UINT16_MAX;
        uint16_t py = (iy < fy->cnt) ? fy->prime[iy] : UINT16_MAX;
        if (px == py) {
            int e = fx->exp[ix] + fy->exp[iy];
            /* compute (p^(e+1)-1)/(p-1) */
            uint64_t p = px;
            uint64_t pow = 1;
            for (int k = 0; k < e + 1; ++k) pow *= p;
            uint64_t term = (pow - 1) / (p - 1);
            sigma *= term;
            ++ix; ++iy;
        } else if (px < py) {
            int e = fx->exp[ix];
            uint64_t p = px;
            uint64_t pow = 1;
            for (int k = 0; k < e + 1; ++k) pow *= p;
            uint64_t term = (pow - 1) / (p - 1);
            sigma *= term;
            ++ix;
        } else { /* py < px */
            int e = fy->exp[iy];
            uint64_t p = py;
            uint64_t pow = 1;
            for (int k = 0; k < e + 1; ++k) pow *= p;
            uint64_t term = (pow - 1) / (p - 1);
            sigma *= term;
            ++iy;
        }
    }
    return sigma;
}

/* ----------------------------------------------------------------------- */
/* Precomputation of best product for every n ≤ N_MAX -------------------- */
static seq_t best_product[N_MAX + 1];
static uint64_t best_sigma_arr[N_MAX + 1];
static bool precomputed = false;

static void precompute(void) {
    if (precomputed) return;
    precomputed = true;
    init_primes_and_factors();

    best_sigma_arr[0] = 0;
    best_product[0] = (seq_t)0;

    for (unsigned n = 1; n <= N_MAX; ++n) {
        /* inherit best from previous n */
        uint64_t cur_best_sigma = best_sigma_arr[n - 1];
        uint64_t cur_best_prod  = (uint64_t)best_product[n - 1];

        /* add all new pairs (x,n) with x ≤ n */
        for (unsigned x = 1; x <= n; ++x) {
            uint64_t prod = (uint64_t)x * (uint64_t)n;
            uint64_t sig  = sigma_of_product(x, n);
            if (sig > cur_best_sigma ||
                (sig == cur_best_sigma && prod < cur_best_prod)) {
                cur_best_sigma = sig;
                cur_best_prod  = prod;
            }
        }
        best_sigma_arr[n] = cur_best_sigma;
        best_product[n]   = (seq_t)cur_best_prod;
    }
}

/* ----------------------------------------------------------------------- */
/* public API ------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n <= N_MAX) {
        precompute();
        return best_product[(unsigned)n];
    }
    /* fallback for n > N_MAX: brute‑force (inefficient but not used) */
    uint64_t max_sigma = 0;
    uint64_t best_prod = 0;
    for (uint64_t x = 1; x <= (uint64_t)n; ++x) {
        for (uint64_t y = 1; y <= (uint64_t)n; ++y) {
            uint64_t prod = x * y;
            /* simple trial division to compute σ(prod) */
            uint64_t tmp = prod;
            uint64_t sigma = 1;
            for (uint64_t p = 2; p * p <= tmp; ++p) {
                if (tmp % p == 0) {
                    uint64_t p_pow = 1;
                    uint64_t sigma_p = 1;
                    while (tmp % p == 0) {
                        tmp /= p;
                        p_pow *= p;
                        sigma_p += p_pow;
                    }
                    sigma *= sigma_p;
                }
            }
            if (tmp > 1) sigma *= (tmp + 1);
            if (sigma > max_sigma || (sigma == max_sigma && prod < best_prod)) {
                max_sigma = sigma;
                best_prod = prod;
            }
        }
    }
    return (seq_t)best_prod;
}
