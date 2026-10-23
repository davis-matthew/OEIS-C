/*
 * A151245  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 0), (0, 1,
 *          0), (0, 1, 1), (1, 0, 0), (1, 0, 1)}.
 * https://oeis.org/A151245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = number of walks of length n in the first octant N^3
 *            starting at (0,0,0) and using steps { (-1,-1,0), (0,1,0),
 *            (0,1,1), (1,0,0), (1,0,1) }
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code computes a small prefix of the sequence by dynamic programming
   and then derives a linear recurrence with Berlekamp-Massey over several
   primes.  The recurrence is applied to evaluate a(n) for any n. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* Modular exponentiation (uint64_t version). */
static uint64_t mod_pow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    while (e) {
        if (e & 1) res = (unsigned __int128)res * a % mod;
        a = (unsigned __int128)a * a % mod;
        e >>= 1;
    }
    return res;
}

/* Modular inverse (mod is prime). */
static uint64_t mod_inv_uint64(uint64_t a, uint64_t mod)
{
    return mod_pow_uint64(a % mod, mod - 2, mod);
}

/* Berlekamp-Massey over a prime modulus.  rec[0] = 1;
   The recurrence is: 0 = rec[0]*s[n] + rec[1]*s[n-1] + ... + rec[L]*s[n-L] */
static void berlekamp_massey(const uint64_t *s, size_t n, uint64_t mod,
                             uint64_t *rec, size_t *rec_len)
{
    uint64_t C[256]   = {0}, B[256] = {0};
    size_t L = 0, m = 1;
    C[0] = B[0] = 1;

    for (size_t i = 0; i < n; ++i) {
        uint64_t d = 0;
        for (size_t j = 0; j <= L; ++j)
            d = (d + C[j] * s[i - j]) % mod;

        if (d == 0) { ++m; continue; }

        uint64_t coef = d * mod_inv_uint64(B[m - 1], mod) % mod;
        uint64_t T[256];
        memcpy(T, C, sizeof(T));

        /* Shift B by m and add to C with coefficient. */
        for (size_t j = 0; j < 256; ++j) {
            uint64_t bj = (j < 256) ? B[j] * coef % mod : 0;
            uint64_t cj = (j + m < 256) ? C[j + m] : 0;
            C[j + m] = (cj + mod - bj) % mod;
        }

        if (2 * L <= i) {
            L = i + 1 - L;
            memcpy(B, T, sizeof(T));
            m = 1;
        } else {
            ++m;
        }
    }

    *rec_len = L;
    rec[0] = 1;
    for (size_t i = 1; i <= L; ++i) rec[i] = C[i];
}

/* Chinese remainder to recover integer from residues mod a small set of primes.
   The result is returned as int64_t (sufficient for the recurrence coeffs). */
static int64_t crt_residue(const uint64_t *res, const uint64_t *primes, size_t k)
{
    uint64_t x = 0;   /* current solution */
    uint64_t m = 1;   /* product of processed primes */

    for (size_t i = 0; i < k; ++i) {
        uint64_t r = res[i];
        if (i > 0) {
            uint64_t inv = mod_inv_uint64(m % primes[i], primes[i]);
            uint64_t t = (r + primes[i] - (x % primes[i])) % primes[i];
            t = (unsigned __int128)t * inv % primes[i];
            x += m * t;
            m *= primes[i];
        } else {
            x = r;
        }
    }
    /* Choose representation with smallest absolute value. */
    if (x > m / 2) x -= m;
    return (int64_t)x;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* ---------- 1. Static initialization on first call ---------- */
    static seq_t base[256];          /* prefix values */
    static seq_t coeff[256];         /* recurrence coefficients: coeff[i] = -rec[i+1] */
    static size_t K = 0;             /* recurrence order */
    static int initialized = 0;

    if (!initialized) {
        const size_t PREFIX = 200;           /* length of computed prefix */
        const size_t DIM = PREFIX + 2;       /* coordinate range needed */

        /* Dynamic programming to compute the prefix. */
        seq_t *dp[2];
        size_t SZ = DIM * DIM;
        dp[0] = (seq_t*)calloc(SZ, sizeof(seq_t));
        dp[1] = (seq_t*)calloc(SZ, sizeof(seq_t));
        if (!dp[0] || !dp[1]) {
            free(dp[0]); free(dp[1]);
            return (seq_t)0;
        }

        dp[0][0] = 1;            /* a(0) = 1 */
        base[0] = 1;

        for (size_t step = 0; step < PREFIX; ++step) {
            size_t cur = step & 1, nxt = cur ^ 1;
            memset(dp[nxt], 0, SZ * sizeof(seq_t));

            for (size_t x = 0; x <= step; ++x) {
                for (size_t y = 0; y <= step; ++y) {
                    seq_t v = dp[cur][x * DIM + y];
                    if (!v) continue;

                    /* (0,1,0) and (0,1,1) : two ways to increase y */
                    dp[nxt][x * DIM + (y + 1)] += v * 2;

                    /* (1,0,0) and (1,0,1) : two ways to increase x */
                    dp[nxt][(x + 1) * DIM + y] += v * 2;

                    /* (-1,-1,0) : decreases x and y if possible */
                    if (x > 0 && y > 0)
                        dp[nxt][(x - 1) * DIM + (y - 1)] += v;
                }
            }

            /* Sum all reachable points after this step. */
            seq_t sum = 0;
            for (size_t x = 0; x <= step + 1; ++x)
                for (size_t y = 0; y <= step + 1; ++y)
                    sum += dp[nxt][x * DIM + y];
            base[step + 1] = sum;
        }

        free(dp[0]); free(dp[1]);

        /* ---------- 2. Determine recurrence via Berlekamp-Massey ---------- */
        const uint64_t primes[3] = {1000003ULL, 1000033ULL, 1000037ULL};
        uint64_t recs[3][256];
        size_t rec_lens[3] = {0};

        for (int pi = 0; pi < 3; ++pi) {
            uint64_t seq_mod[256];
            for (size_t i = 0; i <= PREFIX; ++i)
                seq_mod[i] = (uint64_t)(base[i] % primes[pi]);
            berlekamp_massey(seq_mod, PREFIX + 1, primes[pi], recs[pi], &rec_lens[pi]);
        }

        K = rec_lens[0];
        for (int pi = 1; pi < 3; ++pi)
            if (rec_lens[pi] > K) K = rec_lens[pi];

        /* ---------- 3. Recover integer recurrence coefficients ---------- */
        uint64_t residues[256][3];
        for (size_t j = 1; j <= K; ++j) {
            for (int pi = 0; pi < 3; ++pi) {
                uint64_t r = recs[pi][j];
                residues[j - 1][pi] = (primes[pi] - r) % primes[pi]; /* -rec[j] mod p */
            }
        }

        for (size_t j = 0; j < K; ++j) {
            int64_t val = crt_residue(residues[j], primes, 3);
            coeff[j] = (seq_t)val;
        }

        initialized = 1;
    }

    /* ---------- 4. Evaluate a(n) using the recurrence ---------- */
    if (n < (idx_t)K)
        return base[(size_t)n];

    size_t sz = (size_t)n + 1;
    seq_t *arr = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!arr) return (seq_t)0;

    for (size_t i = 0; i < K; ++i)
        arr[i] = base[i];

    for (size_t i = K; i <= (size_t)n; ++i) {
        seq_t val = 0;
        for (size_t j = 0; j < K; ++j)
            val += coeff[j] * arr[i - 1 - j];
        arr[i] = val;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
