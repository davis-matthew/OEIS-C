/*
 * A216049  Consider the ordered Goldbach partitions of the even numbers m.
 *          Then a(n) is the least m which contains 2n-1 such partitions
 *          composed of odd primes.
 * https://oeis.org/A216049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A216049: a(n) is the smallest even integer m such that the
   number of ordered Goldbach partitions of m composed of odd primes
   equals 2*n-1.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External types provided by the build system */
typedef uint64_t idx_t;               /* index type   */
                                         /* seq_t is an unsigned integer type (uint8_t ... uint64_t, unsigned __int128) */

/* ---------- parameters ---------- */
#define MAX_N   30000            /* pre‑compute enough terms */
#define M       6000000          /* maximal even number to examine           */

/* ---------- small helpers ---------- */
static uint32_t mod_pow(uint32_t a, uint32_t e, uint32_t mod)
{
    uint64_t r = 1, x = a % mod;
    while (e) {
        if (e & 1) r = (r * x) % mod;
        x = (x * x) % mod;
        e >>= 1;
    }
    return (uint32_t)r;
}

/* ---------- NTT on modulus 167772161 (3 is a primitive root for 2^25) ---------- */
#define MOD 167772161U
#define G   3U

static void ntt(uint32_t * a, size_t n, bool invert)
{
    /* bit-reversal permutation */
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        if (i < j) {
            uint32_t tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
        }
    }

    for (size_t len = 2; len <= n; len <<= 1) {
        uint32_t wlen = mod_pow(G, (MOD - 1) / len, MOD);
        if (invert) wlen = mod_pow(wlen, MOD - 2, MOD);
        for (size_t i = 0; i < n; i += len) {
            uint32_t w = 1;
            for (size_t j = 0; j < len / 2; ++j) {
                uint32_t u = a[i + j];
                uint32_t v = (uint32_t)((uint64_t)a[i + j + len / 2] * w % MOD);
                a[i + j] = u + v < MOD ? u + v : u + v - MOD;
                a[i + j + len / 2] = u >= v ? u - v : u + MOD - v;
                w = (uint32_t)((uint64_t)w * wlen % MOD);
            }
        }
    }
    if (invert) {
        uint32_t inv_n = mod_pow((uint32_t)n, MOD - 2, MOD);
        for (size_t i = 0; i < n; ++i)
            a[i] = (uint32_t)((uint64_t)a[i] * inv_n % MOD);
    }
}

/* ---------- main sequence data ---------- */
static seq_t *a_vals = NULL;      /* array of a(n), 1‑based */
static bool initialized = false;  /* once */
static void build_sequence(void)
{
    /* ---------- sieve of Eratosthenes up to M ---------- */
    uint8_t *is_prime = (uint8_t *)calloc(M + 1, sizeof(uint8_t));
    for (size_t i = 2; i <= M; ++i) is_prime[i] = 1;
    for (size_t i = 2; (size_t)i * i <= M; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= M; j += i)
                is_prime[j] = 0;

    /* ---------- prepare convolution array ---------- */
    size_t nfft = 1;
    while (nfft < 2 * M + 1) nfft <<= 1;   /* power of two, at least 2*M+1 */
    uint32_t *arr = (uint32_t *)calloc(nfft, sizeof(uint32_t));

    /* mark odd primes (excluding 2) */
    for (size_t i = 3; i <= M; i += 2)
        if (is_prime[i]) arr[i] = 1;

    /* ---------- NTT, square, inverse NTT ----------
        arr now contains ordered Goldbach partition counts for all indices.   */
    ntt(arr, nfft, false);                    /* forward transform */

    for (size_t i = 0; i < nfft; ++i)         /* element‑wise square */
        arr[i] = (uint32_t)((uint64_t)arr[i] * arr[i] % MOD);

    ntt(arr, nfft, true);                     /* inverse transform */

    /* ---------- allocate result array ---------- */
    a_vals = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));

    /* ---------- populate a_vals ----------
       Only even m with m/2 prime (sigma=1) can yield odd ordered count.   */
    size_t found = 0;
    for (size_t p = 3; p <= M / 2; p += 2) {
        if (!is_prime[p]) continue;
        size_t m = 2 * p;                 /* even number */
        uint32_t ord = arr[m];            /* ordered count (exact, < MOD) */
        if (!(ord & 1U)) continue;        /* should be odd when m/2 is prime */

        uint32_t f = (ord + 1U) / 2U;     /* unordered count = f = n */
        if (f == 0U) continue;
        if (f > (uint32_t)MAX_N) continue;
        if (a_vals[(size_t)f] == 0) {
            a_vals[(size_t)f] = (seq_t)m;
            ++found;
            if (found >= (size_t)MAX_N) break;
        }
    }

    /* ---------- clean up ---------- */
    free(arr);
    free(is_prime);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) build_sequence();
    if (n < 1 || n > (idx_t)MAX_N) return (seq_t)0;   /* out of pre‑computed range */
    return a_vals[(size_t)n];
}
