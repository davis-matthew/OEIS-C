/*
 * A066755  Numbers m such that m^2 + 1 is not divisible by k^2 + 1 for any k
 *          in [1,m-1].
 * https://oeis.org/A066755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = the n-th positive integer m such that for all k with 1 ≤
 *            k < m, (m^2 + 1) mod (k^2 + 1) ≠ 0
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define PRIMELIMIT 50000     /* enough for sqrt(2·10^9) ≈ 44721 */
static int primes[6000];
static int primes_cnt = 0;
static bool primes_initialized = false;

/* Simple sieve to generate all primes ≤ PRIMELIMIT */
static void init_primes(void) {
    if (primes_initialized) return;
    int limit = PRIMELIMIT;
    int *is_comp = (int*)calloc(limit + 1, sizeof(int));
    for (int i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (int j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
    for (int i = 2; i <= limit; ++i)
        if (!is_comp[i])
            primes[primes_cnt++] = i;
    primes_initialized = true;
    free(is_comp);
}

/* Integer square root via Newton's method (no floating point). */
static uint64_t isqrt_u64(uint64_t n) {
    if (n < 2) return n;
    uint64_t x = n;
    while (1) {
        uint64_t y = (x + n / x) >> 1;
        if (y >= x) return x;
        x = y;
    }
}

/* Return true if m^2+1 has no divisor of the form k^2+1 with k<m. */
static bool is_good_value(uint64_t x, uint64_t m) {
    init_primes();

    /* Factor x into primes (small size, at most 16 distinct primes). */
    int pf[16];
    int exps[16];
    int sz = 0;
    uint64_t tmp = x;
    for (int i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        int p = primes[i];
        if (tmp % p == 0) {
            int e = 0;
            do { tmp /= p; e++; } while (tmp % p == 0);
            pf[sz] = p;  exps[sz] = e;  ++sz;
        }
    }
    if (tmp > 1) {
        pf[sz] = tmp; exps[sz] = 1; ++sz;
    }

    /* Enumerate all divisors of x using an explicit stack. */
    struct Node { int idx; uint64_t cur; };
    static struct Node stack[4000];
    int sp = 0;
    stack[sp++] = (struct Node){0,1};

    bool bad = false;
    while (sp) {
        struct Node node = stack[--sp];
        int i = node.idx;
        uint64_t cur = node.cur;

        if (i == sz) {
            if (cur == 1) continue;          /* skip divisor 1 */
            uint64_t d = cur;
            uint64_t s = d - 1;
            uint64_t k = isqrt_u64(s);
            if (k * k == s && k < m) {        /* found a blocking k */
                bad = true;
                break;
            }
        } else {
            uint64_t curP = 1;
            for (int e = 0; e <= exps[i]; ++e) {
                stack[sp++] = (struct Node){i + 1, cur * curP};
                curP *= pf[i];
            }
        }
    }
    return !bad;   /* good iff no bad divisor was found */
}

/* Cached sequence storage. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;          /* next m to test */

/* Return the n-th term of A066755. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* undefined, return 0 */

    while (cache_len < n) {
        uint64_t m = next_candidate++;
        uint64_t x = (uint64_t)m * m + 1;

        if (is_good_value(x, m)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)m;
        }
    }
    return cache[n - 1];
}
