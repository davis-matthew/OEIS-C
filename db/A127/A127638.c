/*
 * A127638  A054525 * A127640, where A127640 = infinite lower triangular
 *          matrix with the sequence of primes in the main diagonal and the
 *          rest zeros.
 * https://oeis.org/A127638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, easy, sign
 * Formula:   a(n) = μ(i/j) * p_j if j|i, else 0, where i is the row number
 *            with 1 + 2 + … + i ≥ n, j = n – (i‑1)i/2, and p_j is the j‑th
 *            prime
 * Range:     i8 n <= 112, i16 n <= 112, i32 n <= 112, i64 n <= 112,
 *            i128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* ----- Basic utilities ----- */

static idx_t isqrt_u64(idx_t x)
{
    /* integer square root, floor(sqrt(x)) */
    idx_t r = 0;
    idx_t bit = ((idx_t)1 << 62);      /* largest power of four ≤ 2^64-1 */
    while (bit > 0) {
        if (r + bit <= (idx_t)(x - r)/2) { /* avoid overflow in r+bit <= x */
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ----- Precomputation of primes and Möbius values ----- */

#define MAX_N   2000

static bool init_done = false;
static int mu_arr[MAX_N + 1];
static int primes_arr[MAX_N];
static int primes_cnt = 0;

static void init_arrays(void)
{
    if (init_done) return;
    static bool comp[MAX_N + 1];
    int pcnt = 0;
    mu_arr[1] = 1;

    for (int i = 2; i <= MAX_N; ++i) {
        if (!comp[i]) {
            primes_arr[pcnt++] = i;
            mu_arr[i] = -1;
        }
        for (int j = 0; j < pcnt; ++j) {
            int p = primes_arr[j];
            int v = i * p;
            if (v > MAX_N) break;
            comp[v] = true;
            if (i % p == 0) {
                mu_arr[v] = 0;
                break;
            } else {
                mu_arr[v] = -mu_arr[i];
            }
        }
    }
    primes_cnt = pcnt;
    init_done = true;
}

/* ----- Sequence function ----- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, unused */

    init_arrays();

    /* find row i: smallest i with i*(i+1)/2 ≥ n */
    idx_t i = (isqrt_u64(1 + 8 * n) - 1) / 2;
    while ((idx_t)((__int128)i * (i + 1) / 2) < n) ++i;
    while (i > 0 && (idx_t)((__int128)(i - 1) * i / 2) >= n) --i;

    /* column j within the row */
    idx_t t  = (idx_t)((__int128)(i - 1) * i / 2);
    idx_t j  = n - t;                   /* 1 ≤ j ≤ i */

    if (i % j != 0) return 0;

    idx_t q = i / j;                    /* q ≤ i ≤ MAX_N */
    int mu = mu_arr[(int)q];
    if (mu == 0) return 0;

    seq_t prime = (seq_t)primes_arr[(int)j - 1];
    if (mu == -1) prime = -prime;
    return prime;
}
