/*
 * A172245  a(n) = number of ways of writing n = i+j such that i<=j,
 *          gcd(i,j,n)=1, and the values of N(i,j,n) are distinct, where
 *          N(i,j,n) = product of distinct prime divisors of i*j*n.
 * https://oeis.org/A172245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of appearances of distinct N(i,j,n) * for all
 *            partitions n = i + j with i <= j, * gcd(i,j,n) = 1, where
 *            N(i,j,n) = product of distinct primes of i·j·n
 * Range:     u8 n <= 540, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"

/* FOR USE IN C++ COMPILATION WITH AN ARBITRARY‑PRECISION seq_t CLASS */
extern seq_t gcd(seq_t a, seq_t b);                      /* from utils/gcd.h */
/* Assume seq_t and idx_t are defined in the build environment */

#define MAXN 10000                                   /* maximum n tested */
static const size_t WORDS = (1229 + 63) / 64;        /* #primes <= MAXN is 1229 */

/* Precomputed data */
static uint64_t *bitmask = NULL;      /* (MAXN+1) * WORDS words of prime‑bitsets */
static size_t   prime_cnt = 0;
static uint32_t prime_list[2000];     /* store primes up to MAXN */
static int      prime_index[MAXN+1];  /* index of prime in prime_list or -1 */
static int      initialized = 0;

/* Euclidean gcd for uint64_t (used during generation). */

/* One‑time global precomputation of primes and bitmasks. */
static void a_init(void)
{
    if (initialized) return;
    initialized = 1;

    /* Sieve of Eratosthenes up to MAXN. */
    uint8_t comp[MAXN + 1] = {0};
    for (int p = 2; p <= MAXN; ++p) {
        if (!comp[p]) {
            prime_list[prime_cnt++] = p;
            prime_index[p] = (int)prime_cnt - 1;
            if ((int64_t)p * p <= MAXN)
                for (int64_t q = (int64_t)p * p; q <= MAXN; q += p)
                    comp[(int)q] = 1;
        }
    }
    /* Mark non‑prime entries. */
    for (int p = 2; p <= MAXN; ++p)
        if (comp[p]) prime_index[p] = -1;

    /* Allocate space for all prime‑bitsets. */
    bitmask = (uint64_t *)malloc((MAXN + 1) * WORDS * sizeof(uint64_t));
    if (!bitmask) return;        /* allocation failure, best effort */

    /* Build the prime‑bitset for each integer 1..MAXN. */
    for (int n = 1; n <= MAXN; ++n) {
        uint64_t *m = bitmask + n * WORDS;
        memset(m, 0, WORDS * sizeof(uint64_t));

        int temp = n;
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            int p = prime_list[pi];
            if (p * p > temp) break;
            if (temp % p == 0) {
                m[pi >> 6] |= (uint64_t)1 << (pi & 63);
                while (temp % p == 0) temp /= p;
            }
        }
        if (temp > 1) {                      /* temp is a prime > sqrt(n) */
            int pi = prime_index[temp];
            m[pi >> 6] |= (uint64_t)1 << (pi & 63);
        }
    }
}

/* Comparator for qsort on the prime‑bitsets. */
static int cmp_masks(const void *a1, const void *a2)
{
    const uint64_t *m1 = (const uint64_t *)a1;
    const uint64_t *m2 = (const uint64_t *)a2;
    for (size_t w = 0; w < WORDS; ++w) {
        if (m1[w] < m2[w]) return -1;
        if (m1[w] > m2[w]) return 1;
    }
    return 0;
}

/* ------------------------------------------------------------------- */
/*     Main sequence function                                         */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    a_init();
    if (!bitmask) return 0;         /* fallback if precomputation failed */

    idx_t half = n / 2;
    size_t max_cnt = (size_t)half;  /* at most half entries */
    /* allocate buffer for masks */
    uint64_t *buf = (uint64_t *)malloc(max_cnt * WORDS * sizeof(uint64_t));
    if (!buf) return 0;

    size_t count = 0;
    for (idx_t i = 1; i <= half; ++i) {
        idx_t j = n - i;
        if (i > j) break;           /* safeguard, should not happen */
        uint64_t g = gcd_u64(gcd_u64((uint64_t)i, (uint64_t)j), (uint64_t)n);
        if (g != 1) continue;       /* gcd condition fails */

        uint64_t *out  = buf + count * WORDS;
        const uint64_t *bi = bitmask + i * WORDS;
        const uint64_t *bj = bitmask + j * WORDS;
        const uint64_t *bn = bitmask + n * WORDS;
        for (size_t w = 0; w < WORDS; ++w)
            out[w] = bi[w] | bj[w] | bn[w];
        ++count;
    }

    if (count == 0) {
        free(buf);
        return 0;
    }

    /* sort masks and count distinct ones */
    qsort(buf, count, WORDS * sizeof(uint64_t), cmp_masks);

    seq_t uniq = 1;
    for (size_t idx = 1; idx < count; ++idx) {
        const uint64_t *prev = buf + (idx - 1) * WORDS;
        const uint64_t *curr = buf + idx * WORDS;
        if (memcmp(prev, curr, WORDS * sizeof(uint64_t)) != 0)
            ++uniq;
    }

    free(buf);
    return uniq;
}
