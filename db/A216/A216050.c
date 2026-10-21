/*
 * A216050  Consider the unordered Goldbach partitions of the even numbers m.
 *          Then a(n) is the least m which contains 2n-1 such partitions
 *          composed of odd primes.
 * https://oeis.org/A216050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least even m that has exactly 2n‑1 unordered Goldbach *
 *            partitions into odd primes. The function builds a prime sieve up
 *            to a * dynamic limit, performs a convolution by NTT to count
 *            ordered prime * sums, converts them to unordered counts, and
 *            records the first n * occurrences of the requir...
 * Range:     u8 n <= 8, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- NTT for modulus 998244353 ---------- */
#define MOD 998244353
#define G 3

static inline int mod_pow(int a, int e)
{
    int r = 1;
    while (e) {
        if (e & 1) r = (int)((long long)r * a % MOD);
        a = (int)((long long)a * a % MOD);
        e >>= 1;
    }
    return r;
}

static void ntt(int *a, size_t n, bool invert)
{
    /* bit reversal */
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        if (i < j) {
            int tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
        }
    }
    /* transform */
    for (size_t len = 2; len <= n; len <<= 1) {
        int wlen = mod_pow(G, (MOD - 1) / (int)len);
        if (invert) wlen = mod_pow(wlen, MOD - 2);
        for (size_t i = 0; i < n; i += len) {
            int w = 1;
            for (size_t j = 0; j < len / 2; ++j) {
                int u = a[i + j];
                int v = (int)((long long)a[i + j + len / 2] * w % MOD);
                int s = u + v;
                if (s >= MOD) s -= MOD;
                int d = u - v;
                if (d < 0) d += MOD;
                a[i + j] = s;
                a[i + j + len / 2] = d;
                w = (int)((long long)w * wlen % MOD);
            }
        }
    }
    if (invert) {
        int inv_n = mod_pow((int)n, MOD - 2);
        for (size_t i = 0; i < n; ++i)
            a[i] = (int)((long long)a[i] * inv_n % MOD);
    }
}

/* ---------- cache ---------------------------------------- */
static seq_t *a_cache = NULL;      /* 1‑based array of a(n) values */
static idx_t  cache_size = 0;      /* number of stored entries (including 0) */

/* ---------- rebuild cache for at least n_target terms ---------- */
static void rebuild_cache(idx_t n_target)
{
    /* choose a sieve limit that will contain enough partitions */
    size_t limit = n_target * 700;            /* heuristic multiplier */
    if (limit < 800) limit = 800;
    /* the modulus 998244353 supports NTT for lengths up to 2^23 */
    const size_t MAX_NTT = 1ULL << 23; /* 8,388,608 */
    size_t max_limit = (MAX_NTT >> 1) - 1;       /* 4,194,303 */
    if (limit > max_limit) limit = max_limit;

    for (;;) { /* repeat if not enough terms found */
        /* size for convolution */
        size_t N = 1;
        while (N <= 2 * limit) N <<= 1;          /* power of two >= 2*limit */

        /* sieve primes up to limit */
        char *is_prime = (char *)calloc(limit + 1, 1);
        if (!is_prime) exit(1);
        memset(is_prime, 1, limit + 1);
        is_prime[0] = is_prime[1] = 0;
        for (size_t i = 2; i * i <= limit; ++i)
            if (is_prime[i])
                for (size_t j = i * i; j <= limit; j += i)
                    is_prime[j] = 0;

        /* build array for convolution (odd primes only) */
        int *A = (int *)calloc(N, sizeof(int));
        if (!A) exit(1);
        for (size_t i = 3; i <= limit; i += 2)
            if (is_prime[i]) A[i] = 1;

        /* convolution via NTT */
        ntt(A, N, false);                     /* forward transform */
        for (size_t i = 0; i < N; ++i)
            A[i] = (int)((long long)A[i] * A[i] % MOD);
        ntt(A, N, true);                      /* inverse transform */

        /* gather sequence terms */
        seq_t *new_cache = (seq_t *)calloc(n_target + 1, sizeof(seq_t));
        if (!new_cache) exit(1);
        idx_t found = 0;

        for (size_t m = 6; m <= limit; m += 2) {
            int ordered = A[m];
            /* correct for self-pair if m/2 is an odd prime */
            bool center = false;
            size_t half = m / 2;
            if ((half & 1) != 0 && half <= limit && is_prime[half])
                center = true;
            int unordered = (ordered + (center ? 1 : 0)) / 2;
            if ((unordered & 1) == 0) continue;     /* need odd count */
            idx_t idx = (unordered + 1) >> 1;        /* (2n-1) → n */
            if (idx <= n_target && new_cache[idx] == 0) {
                new_cache[idx] = (seq_t)m;
                ++found;
                if (found == n_target) break;
            }
        }

        /* free temporary arrays */
        free(A);
        free(is_prime);

        if (found >= n_target) {
            free(a_cache);
            a_cache = new_cache;
            cache_size = n_target + 1;
            return;
        }

        /* not enough terms yet: increase limit and try again */
        free(new_cache);
        if (limit == max_limit) /* cannot extend more, give up */
            return;
        limit = limit * 2;
        if (limit > max_limit) limit = max_limit;
    }
}

/* ---------- public interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (cache_size <= n) rebuild_cache(n);
    return a_cache[n];
}
