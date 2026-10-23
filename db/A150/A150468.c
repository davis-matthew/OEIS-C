/*
 * A150468  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 1), (-1, 0,
 *          0), (0, 0, 1), (1, 0, -1), (1, 1, 0)}.
 * https://oeis.org/A150468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = number of length‑n walks in the first octant of ℤ³
 *            starting at (0,0,0) and using steps {(-1,-1,1), (-1,0,0),
 *            (0,0,1), (1,0,-1), (1,1,0)}. The implementation computes ≈80
 *            terms of the sequence modulo several primes, determines its
 *            linear recurrence by Berlekamp–Massey, reconstructs the inte...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Primes for modular arithmetic ---------- */
#define PRIME_COUNT 3
static const uint64_t PRIMES[PRIME_COUNT] =
{ 1000000007ULL, 1000000009ULL, 998244353ULL };

/* ---------- Maximum steps used to determine the recurrence ---------- */
#define MAX_STEPS   80
#define DIM         (MAX_STEPS + 1)
#define TOTAL       (DIM * DIM * DIM)

/* ---------- Recurrence storage ---------- */
#define MAX_ORDER   MAX_STEPS  /* upper bound for recurrence order */
static seq_t init_vals[MAX_ORDER];
static long long rec_coeff[MAX_ORDER];
static size_t  order_global = 0;
static int prepared = 0;

/* ---------- Step set ---------- */
static const int step_vec[5][3] =
{ {-1,-1, 1},
  {-1, 0, 0},
  { 0, 0, 1},
  { 1, 0,-1},
  { 1, 1, 0} };

/* ---------- Modular exponentiation (prime modulus) ---------- */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1) result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}
static uint64_t mod_inv64(uint64_t a, uint64_t mod)
{
    return mod_pow64(a, mod - 2, mod);
}

/* ---------- Berlekamp–Massey over a prime ---------- */
static size_t berlekamp_massey(const uint64_t *s, size_t n,
                               uint64_t *C, uint64_t mod)
{
    uint64_t B[n * 2 + 1];
    for (size_t i = 0; i < 2 * n + 1; ++i) B[i] = 0;
    C[0] = B[0] = 1;
    size_t L = 0, m = 1;
    uint64_t b = 1;

    for (size_t cur = 0; cur < n; ++cur)
    {
        uint64_t d = 0;
        for (size_t i = 0; i <= L; ++i)
        {
            uint64_t term = (unsigned __int128)C[i] * s[cur - i] % mod;
            d += term;
            if (d >= mod) d -= mod;
        }
        if (d == 0)
        {
            ++m;
            continue;
        }
        uint64_t coef = (unsigned __int128)d * mod_inv64(b, mod) % mod;
        if (2 * L <= cur)
        {
            for (size_t i = 0; i <= L; ++i) B[i] = C[i];
            L = cur + 1 - L;
            b = d;
            m = 1;
        }
        for (size_t i = 0; i <= L; ++i)
        {
            uint64_t term = (unsigned __int128)coef * B[i] % mod;
            size_t idx = i + m;
            if (idx > 2 * n) continue;
            uint64_t sub = C[idx] + mod - term;
            if (sub >= mod) sub -= mod;
            C[idx] = sub;
        }
        ++m;
    }
    return L;
}

/* ---------- Garner CRT for a single integer ---------- */
static unsigned __int128 crt_uint128(const uint64_t *rem, size_t pcnt,
                                     const uint64_t *primes)
{
    unsigned __int128 x = rem[0];
    unsigned __int128 prod = primes[0];
    for (size_t i = 1; i < pcnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t prod_mod_p = (uint64_t)(prod % p);
        uint64_t inv = mod_inv64(prod_mod_p, p);
        uint64_t t = ( (rem[i] + p - (uint64_t)(x % p)) % p ) * inv % p;
        x += prod * t;
        prod *= p;
    }
    return x;
}

/* ---------- Sign conversion of CRT result ---------- */
static long long crt_signed(const uint64_t *rem, size_t pcnt,
                            const uint64_t *primes)
{
    unsigned __int128 x = crt_uint128(rem, pcnt, primes);
    unsigned __int128 prod = 1;
    for (size_t i = 0; i < pcnt; ++i) prod *= primes[i];
    unsigned __int128 half = prod >> 1;
    __int128 signed_val = (x > half) ? ( (__int128)(x - prod) ) : (__int128)x;
    return (long long)signed_val;
}

/* ---------- Compute first MAX_STEPS terms modulo a prime ---------- */
static void compute_mod_terms(uint64_t prime, uint64_t *out)
{
    uint64_t *cur = (uint64_t*)malloc(TOTAL * sizeof(uint64_t));
    uint64_t *nxt = (uint64_t*)malloc(TOTAL * sizeof(uint64_t));
    if (!cur || !nxt) abort();

    memset(cur, 0, TOTAL * sizeof(uint64_t));
    cur[0] = 1;
    out[0] = 1 % prime;

    for (int step = 1; step < MAX_STEPS; ++step)
    {
        memset(nxt, 0, TOTAL * sizeof(uint64_t));
        uint64_t sum = 0;
        for (int x = 0; x < step; ++x)
        {
            for (int y = 0; y < step; ++y)
            {
                for (int z = 0; z < step; ++z)
                {
                    uint64_t val = cur[(x * DIM + y) * DIM + z];
                    if (!val) continue;
                    for (int t = 0; t < 5; ++t)
                    {
                        int nx = x + step_vec[t][0];
                        int ny = y + step_vec[t][1];
                        int nz = z + step_vec[t][2];
                        if (nx < 0 || ny < 0 || nz < 0 ||
                            nx > step || ny > step || nz > step) continue;
                        size_t idx = (nx * DIM + ny) * DIM + nz;
                        uint64_t nv = nxt[idx] + val;
                        if (nv >= prime) nv -= prime;
                        nxt[idx] = nv;
                        sum += val;
                        if (sum >= prime) sum -= prime;
                    }
                }
            }
        }
        out[step] = sum;
        memcpy(cur, nxt, TOTAL * sizeof(uint64_t));
    }

    free(cur);
    free(nxt);
}

/* ---------- Prepare recurrence and initial values ---------- */
static void prepare(void)
{
    /* arrays for modular sequences */
    uint64_t seq_mod[PRIME_COUNT][MAX_STEPS];
    uint64_t poly[PRIME_COUNT][MAX_STEPS + 1];
    size_t ord[PRIME_COUNT] = {0};

    /* compute modular sequences */
    for (size_t p = 0; p < PRIME_COUNT; ++p)
        compute_mod_terms(PRIMES[p], seq_mod[p]);

    /* compute BM for each prime */
    for (size_t p = 0; p < PRIME_COUNT; ++p)
        ord[p] = berlekamp_massey(seq_mod[p], MAX_STEPS,
                                  poly[p], PRIMES[p]);

    /* choose maximal order */
    for (size_t p = 0; p < PRIME_COUNT; ++p)
        if (ord[p] > order_global) order_global = ord[p];

    /* reconstruct coefficients (negative of BM coefficients) */
    for (size_t i = 0; i < order_global; ++i) /* i 0..L-1 correspond to a[n-i-1] */
    {
        uint64_t rem[PRIME_COUNT];
        for (size_t p = 0; p < PRIME_COUNT; ++p)
            rem[p] = (i + 1 <= ord[p]) ? poly[p][i + 1] : 0;
        /* coefficient for a[n-(i+1)] in recurrence: -c[i+1] */
        rec_coeff[i] = -crt_signed(rem, PRIME_COUNT, PRIMES);
    }

    /* reconstruct initial terms 0..order_global-1 */
    for (size_t i = 0; i < order_global; ++i)
    {
        uint64_t rem[PRIME_COUNT];
        for (size_t p = 0; p < PRIME_COUNT; ++p)
            rem[p] = seq_mod[p][i];
        unsigned __int128 v = crt_uint128(rem, PRIME_COUNT, PRIMES);
        init_vals[i] = (seq_t)v;
    }

    prepared = 1;
}

/* ---------- Public function required by the problem ---------- */
seq_t a(idx_t n)
{
    if (!prepared) prepare();

    if ((idx_t)order_global > n) return init_vals[n];

    seq_t buf[MAX_ORDER];
    for (size_t i = 0; i < order_global; ++i) buf[i] = init_vals[i];

    for (idx_t step = order_global; step <= n; ++step)
    {
        __int128 acc = 0;
        for (size_t i = 0; i < order_global; ++i)
            acc += (__int128)rec_coeff[i] *
                   (__int128)buf[order_global - 1 - i];
        buf[order_global - 1] = (seq_t)acc;
    }
    return buf[order_global - 1];
}
