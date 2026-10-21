/*
 * A227920  Number of ways to write n = x + y + z with y and z distinct and
 *          greater than x such that 6*x-1, 6*y-1, 6*x*y-1 are Sophie Germain
 *          primes and {6*x-1, 6*x+1}, {6*z-1, 6*z+1}, {6*x*z-1, 6*x*z+1} are
 *          twin prime pairs.
 * https://oeis.org/A227920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence  A227920
   a(n) = number of representations n = x + y + z
   with y and z distinct and > x, 6x-1, 6y-1, 6xy-1 Sophie Germain primes
   and {6x-1,6x+1}, {6z-1,6z+1}, {6xz-1,6xz+1} twin prime pairs. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* upper bounds for precomputation */
#define MAX_N          10000            /* maximum n queried */
#define PRIMES_UPPER   50000            /* sqrt(12 * MAX_N^2) < 50000 */

/* pre–computed primes up to PRIMES_UPPER */
static uint32_t prime_list[5300];        /* enough to hold all primes <= 50000 */
static size_t   prime_cnt = 0;

/* flags for x,y,z validity */
static bool validX[MAX_N + 1] = {false};
static bool validY[MAX_N + 1] = {false};
static bool validZ[MAX_N + 1] = {false};

/* lists of valid indices */
static uint16_t Xlist[2000];
static uint16_t Ylist[2000];
static size_t   Xcnt = 0, Ycnt = 0;

static bool init_done = false;

/* ---------- simple sieve up to PRIMES_UPPER ---------- */
static void build_primes(void)
{
    bool is_comp[PRIMES_UPPER + 1] = {false};
    for (uint32_t i = 2; i <= PRIMES_UPPER; ++i) {
        if (!is_comp[i]) {
            prime_list[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIMES_UPPER) {
                for (uint32_t j = i * i; j <= PRIMES_UPPER; j += i)
                    is_comp[j] = true;
            }
        }
    }
}

/* ---------- primality test for unsigned 32‑bit numbers ---------- */
static bool is_prime_uint32(uint32_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ---------- initialise all tables ---------- */
static void init_precompute(void)
{
    build_primes();

    for (uint32_t x = 1; x <= MAX_N; ++x) {
        uint32_t p1 = 6U * x - 1;   /* 6x-1 */
        uint32_t p2 = 6U * x + 1;   /* 6x+1 (twin) */
        uint32_t p3 = 12U * x - 1;  /* 12x-1  (Sophie Germain) */

        if (is_prime_uint32(p1) && is_prime_uint32(p2) && is_prime_uint32(p3)) {
            validX[x] = true;
            Xlist[Xcnt++] = (uint16_t)x;
        }
    }

    for (uint32_t y = 1; y <= MAX_N; ++y) {
        uint32_t p1 = 6U * y - 1;
        uint32_t p3 = 12U * y - 1;
        if (is_prime_uint32(p1) && is_prime_uint32(p3)) {
            validY[y] = true;
            Ylist[Ycnt++] = (uint16_t)y;
        }
    }

    for (uint32_t z = 1; z <= MAX_N; ++z) {
        uint32_t p1 = 6U * z - 1;
        uint32_t p2 = 6U * z + 1;
        if (is_prime_uint32(p1) && is_prime_uint32(p2))
            validZ[z] = true;
    }

    init_done = true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (!init_done) init_precompute();

    if (n < 6)   /* smallest admissible triple is 1+2+3 */
        return (seq_t)0;

    uint64_t count = 0ULL;

    for (size_t ix = 0; ix < Xcnt; ++ix) {
        uint32_t x = Xlist[ix];

        /* minimal possible sum with this x is 3x+3 */
        if ((uint64_t)3 * x + 3 > n) continue;

        for (size_t iy = 0; iy < Ycnt; ++iy) {
            uint32_t y = Ylist[iy];
            if (y <= x) continue;                /* y must exceed x */

            uint32_t rem = (uint32_t)n - x;       /* remaining sum for y+z */
            if (rem <= y) continue;               /* ensures z>0 and z!=y */

            uint32_t z = rem - y;                 /* candidate z */
            if (z <= x || z == y) continue;       /* z must be >x and distinct */
            if (z > MAX_N || !validZ[z]) continue;/* z must be pre‑computed and twin */

            /* 6*x*y-1 prime and its SG partner 12*x*y-1 */
            uint64_t prod_xy = (uint64_t)6 * x * y;
            uint32_t p_xy = (uint32_t)(prod_xy - 1);
            if (!is_prime_uint32(p_xy)) continue;

            uint32_t p_xy2 = (uint32_t)((uint64_t)12 * x * y - 1);
            if (!is_prime_uint32(p_xy2)) continue;

            /* 6*x*z-1 prime and twin 6*x*z+1 */
            uint64_t prod_xz = (uint64_t)6 * x * z;
            uint32_t p_xz = (uint32_t)(prod_xz - 1);
            if (!is_prime_uint32(p_xz)) continue;

            uint32_t p_xz2 = p_xz + 2;
            if (!is_prime_uint32(p_xz2)) continue;

            ++count;
        }
    }

    return (seq_t)count;
}
