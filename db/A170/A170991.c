/*
 * A170991  Number of genus 2, degree n, simply ramified covers of an elliptic
 *          curve.
 * https://oeis.org/A170991
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in (5*E2^3 - 3*E2*E4 - 2*E6)/25920 ,
 *            where E_k are the Eisenstein series of weight k
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* --------------  cache -------------- */
static idx_t recomputed = 0;

static signed __int128 *sigma1_arr = NULL;
static signed __int128 *sigma3_arr = NULL;
static signed __int128 *sigma5_arr = NULL;
static signed __int128 *e2_arr    = NULL;
static signed __int128 *e4_arr    = NULL;
static signed __int128 *e6_arr    = NULL;
static signed __int128 *e2sq_arr  = NULL;
static signed __int128 *e2cub_arr = NULL;
static signed __int128 *e2e4_arr  = NULL;
static seq_t          *a_arr      = NULL;

/* --------------  helpers -------------- */
static void free_cache(void)
{
    free(sigma1_arr); sigma1_arr = NULL;
    free(sigma3_arr); sigma3_arr = NULL;
    free(sigma5_arr); sigma5_arr = NULL;
    free(e2_arr);    e2_arr    = NULL;
    free(e4_arr);    e4_arr    = NULL;
    free(e6_arr);    e6_arr    = NULL;
    free(e2sq_arr);  e2sq_arr  = NULL;
    free(e2cub_arr); e2cub_arr = NULL;
    free(e2e4_arr);  e2e4_arr = NULL;
    free(a_arr);     a_arr     = NULL;
}

/* --------------  main recompute -------------- */
static void recompute(idx_t N)
{
    if (N < 2) return;
    free_cache();

    size_t sz = (size_t)(N + 1);

    sigma1_arr = (signed __int128 *)calloc(sz, sizeof(signed __int128));
    sigma3_arr = (signed __int128 *)calloc(sz, sizeof(signed __int128));
    sigma5_arr = (signed __int128 *)calloc(sz, sizeof(signed __int128));
    e2_arr    = (signed __int128 *)calloc(sz, sizeof(signed __int128));
    e4_arr    = (signed __int128 *)calloc(sz, sizeof(signed __int128));
    e6_arr    = (signed __int128 *)calloc(sz, sizeof(signed __int128));
    e2sq_arr  = (signed __int128 *)calloc(sz, sizeof(signed __int128));
    e2cub_arr = (signed __int128 *)calloc(sz, sizeof(signed __int128));
    e2e4_arr  = (signed __int128 *)calloc(sz, sizeof(signed __int128));
    a_arr     = (seq_t          *)calloc(sz, sizeof(seq_t));

    /* divisor sums */
    for (idx_t d = 1; d <= N; ++d) {
        signed __int128 dval = (signed __int128)d;
        signed __int128 d2   = dval * dval;
        signed __int128 d3   = d2 * dval;
        signed __int128 d5   = d3 * d2;
        for (idx_t m = d; m <= N; m += d) {
            size_t i = (size_t)m;
            sigma1_arr[i] += dval;
            sigma3_arr[i] += d3;
            sigma5_arr[i] += d5;
        }
    }

    /* Eisenstein series coefficients */
    e2_arr[0] = 1;
    e4_arr[0] = 1;
    e6_arr[0] = 1;
    for (size_t i = 1; i <= (size_t)N; ++i) {
        e2_arr[i] = -((signed __int128)24 * sigma1_arr[i]);
        e4_arr[i] =  ((signed __int128)240 * sigma3_arr[i]);
        e6_arr[i] = -((signed __int128)504 * sigma5_arr[i]);
    }

    /* e2 * e2  -> e2sq */
    for (size_t i = 0; i <= (size_t)N; ++i) {
        signed __int128 si = e2_arr[i];
        if (si == 0) continue;
        for (size_t j = 0; j + i <= (size_t)N; ++j) {
            e2sq_arr[i + j] += si * e2_arr[j];
        }
    }

    /* e2sq * e2 -> e2cub */
    for (size_t i = 0; i <= (size_t)N; ++i) {
        signed __int128 si = e2sq_arr[i];
        if (si == 0) continue;
        for (size_t j = 0; j + i <= (size_t)N; ++j) {
            e2cub_arr[i + j] += si * e2_arr[j];
        }
    }

    /* e2 * e4 -> e2e4 */
    for (size_t i = 0; i <= (size_t)N; ++i) {
        signed __int128 si = e2_arr[i];
        if (si == 0) continue;
        for (size_t j = 0; j + i <= (size_t)N; ++j) {
            e2e4_arr[i + j] += si * e4_arr[j];
        }
    }

    /* compute a(n) */
    const signed __int128 div = (signed __int128)25920;
    for (size_t i = 2; i <= (size_t)N; ++i) {
        signed __int128 num = 5 * e2cub_arr[i]
                           - 3 * e2e4_arr[i]
                           - 2 * e6_arr[i];
        seq_t val = (seq_t)(num / div);
        a_arr[i] = val;
    }

    recomputed = N;
}

/* --------------  public function -------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;
    if (n > recomputed) recompute(n);
    return a_arr[(size_t)n];
}
